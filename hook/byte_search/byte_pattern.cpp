#include "byte_pattern.h"

using namespace std;
using namespace std::filesystem;

// log

std::ofstream& BytePattern::log_stream(){
	static ofstream instance;
	return instance;
}

void BytePattern::StartLog(const char* module_name){
	ShutdownLog();

	// reference
	// https://qiita.com/maaato414/items/70790af74e0cae39b11d
	char filename[256];
	char prg_path[256];

	readlink("/proc/self/exe", prg_path, sizeof(prg_path));
	sprintf(filename ,"pattern_%s.log", module_name);

	log_stream().open(path{ prg_path }.parent_path() / filename, ios::trunc);
}

void BytePattern::ShutdownLog(){
	log_stream().close();
}

void BytePattern::debug_output() const{
	if (!log_stream().is_open())
		return;

	log_stream() << hex << uppercase;
	log_stream() << "Result(s) of pattern: " << _literal << '\n';

	if (count() > 0){
		for_each_result([this](memory_pointer pointer){
			log_stream() << "0x" << pointer.address() << '\n';
		});
	} else {
		log_stream() << "None\n";
	}

	log_stream() << "--------------------------------------------------------------------------------------" << endl;
}

size_t BytePattern::count() const{
	return this->_results.size();
}

bool BytePattern::has_size(size_t expected, string desc) const{
	const bool result = (this->_results.size() == expected);

	LoggingInfo(desc + (result ? ":[OK]" : ":[NG]"));

	return result;
}

// start end

BytePattern& BytePattern::temp_instance(){
	static BytePattern instance;
	return instance;
}

BytePattern::BytePattern(){
	setModule();
}

BytePattern& BytePattern::setModule(){
	// Get self process file name
	// https://qiita.com/maaato414/items/70790af74e0cae39b11d
	char prgName[64] = {0};
	char prgPath[256] = { 0 };

	readlink("/proc/self/exe", prgPath, sizeof(prgPath));
	sprintf(prgName, "%s", basename(prgPath));
	
	// TODO:  findだとテキストが存在するだけで一致してしまうので/とスペースを入れているが自身のプログラムから取得するべき
	return setModule("/ck2 ");
}

BytePattern& BytePattern::setModule(string moduleName){
	this->getModuleRanges(moduleName);
	return *this;
}

void BytePattern::clear(){
	this->_literal.clear();
	this->_pattern.clear();
	this->_mask.clear();
	this->_results.clear();
}

void BytePattern::getModuleRanges(string moduleName){
	_ranges.clear();

	// search self process address from maps

	// ex) 08048000-08053000 r-xp 00000000 fd:01 30433      /bin/cat
	std::string maps = "/proc/self/maps";
	std::fstream fs(maps, std::ios::in);

	std::string line;
	while (std::getline(fs, line)) {
		if (line.find(moduleName) != std::string::npos) {
			printf("%s\n", line.c_str());

			pair<uintptr_t, uintptr_t> range;

			int prot = PROT_READ;
			unsigned long long start, end;

			// TODO: ここは面倒なことが起きる。プログラムによってはADDRLENは8だったり12だったりするので正規表現でやるべき
			range.first = std::stoull(line.substr(0, ADDRLEN), 0, 16);
			range.second = std::stoull(line.substr(ADDRLEN + 1, ADDRLEN), 0, 16);

			if (line[2 * ADDRLEN + 3] == 'w'){
				prot |= PROT_WRITE;
			}

			if (line[2 * ADDRLEN + 4] == 'x'){
				prot |= PROT_EXEC;
			}
			
			if (not(prot & PROT_WRITE)) {
				if (mprotect((void*)range.first, range.second - range.first, prot | PROT_WRITE)) {
					printf("%d", errno);
					exit(errno);
				}
			}

			this->_ranges.emplace_back(range);
		}
	}
}

// pattern

BytePattern& BytePattern::find_pattern(boost::string_view pattern_literal){
	this->set_pattern(pattern_literal).search();

	return *this;
}

BytePattern& BytePattern::set_pattern(boost::string_view pattern_literal){
	this->transform_pattern(pattern_literal);
	this->bm_preprocess();

	return *this;
}

void BytePattern::transform_pattern(boost::string_view literal){
	vector<string> sub_patterns;

	this->clear();
	this->_literal.assign(literal.begin(), literal.end());

	if (literal.empty()){
		return;
	}

	boost::algorithm::split(sub_patterns, _literal, boost::is_any_of(" "));

	try{
		for (auto sub : sub_patterns){
			auto pat = parse_sub_pattern(sub);

			this->_pattern.push_back(pat.first);
			this->_mask.push_back(pat.second);
		}
	}catch (const invalid_argument&){
		this->clear();
	}
}

pair<uint8_t, uint8_t> BytePattern::parse_sub_pattern(boost::string_view sub){
	auto digit_to_value = [](char character) {
		if ('0' <= character && character <= '9') return (character - '0');
		else if ('A' <= character && character <= 'F') return (character - 'A' + 10);
		else if ('a' <= character && character <= 'f') return (character - 'a' + 10);
		throw invalid_argument("Could not parse pattern."); };

	pair<uint8_t, uint8_t> result;

	if (sub.size() == 1){
		if (sub[0] == '?'){
			result.first = 0;
			result.second = 0;
		}else{
			result.first = digit_to_value(sub[0]);
			result.second = 0xFF;
		}
	} else if (sub.size() == 2) {
		if (sub[0] == '?' && sub[1] == '?'){
			result.first = 0;
			result.second = 0;
		} else if (sub[0] == '?') {
			result.first = digit_to_value(sub[1]);
			result.second = 0xF;
		} else if (sub[1] == '?') {
			result.first = (digit_to_value(sub[0]) << 4);
			result.second = 0xF0;
		} else {
			result.first = ((digit_to_value(sub[0]) << 4) | digit_to_value(sub[1]));
			result.second = 0xFF;
		}
	} else {
		throw invalid_argument("Could not parse pattern.");
	}

	return result;
}

void BytePattern::bm_preprocess(){
	ptrdiff_t index;

	const uint8_t* pbytes = this->_pattern.data();
	const uint8_t* pmask = this->_mask.data();
	size_t pattern_len = this->_pattern.size();

	for (uint32_t bc = 0; bc < 256; ++bc){
		for (index = pattern_len - 1; index >= 0; --index){
			if ((pbytes[index] & pmask[index]) == (bc & pmask[index])){
				break;
			}
		}

		this->_bmbc[bc] = index;
	}
}

// search

BytePattern& BytePattern::search(){
	this->bm_search();

	debug_output();

	return *this;
}

void BytePattern::bm_search()
{
	const uint8_t* pbytes = this->_pattern.data();
	const uint8_t* pmask = this->_mask.data();
	size_t pattern_len = this->_pattern.size();

	this->_results.clear();

	if (pattern_len == 0){
		return;
	}

	for (auto& range : this->_ranges){
		uint8_t* range_begin = reinterpret_cast<uint8_t*>(range.first);
		uint8_t* range_end = reinterpret_cast<uint8_t*>(range.second - pattern_len);

		ptrdiff_t index;

		while (range_begin <= range_end){
			for (index = pattern_len - 1; index >= 0; --index){
				if ((pbytes[index] & pmask[index]) != (range_begin[index] & pmask[index])){
					break;
				}
			}

			if (index == -1){
				this->_results.emplace_back(range_begin);
				range_begin += pattern_len;
			}else{
				range_begin += max(index - this->_bmbc[range_begin[index]], 1);
			}
		}
	}
}

memory_pointer BytePattern::get(size_t index) const {
	return this->_results.at(index);
}

memory_pointer BytePattern::get_first() const{
	return this->get(0);
}

memory_pointer BytePattern::get_second() const{
	return this->get(1);
}

void BytePattern::LoggingInfo(const std::string message){
	if (!log_stream().is_open())
		return;

	log_stream() << message << "\n";

	log_stream() << "--------------------------------------------------------------------------------------" << '\n' << endl;
}