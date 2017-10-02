#pragma once
#include <vector>
class HAMM
{
public:
	HAMM();
	HAMM(std::vector<int>& word) : word(word)
	{}
	~HAMM();
	
	void encrypt();
	void decipher();
	void set_word();
	std::vector<int> get_word();
	std::vector<int> get_codeword();
	size_t get_word_size() { return word.size(); }
	size_t get_codeword_size() { return codeword.size(); }
	void show_word();
	void show_codeword();
	void set_codeword_mistake(int pos);
	static void show(std::vector<int> v);

private :
	std::vector<int> word;
	std::vector<int> codeword;

	/** encrypt helper functions */
	std::vector<int> create_pow2_vec(size_t n); // 1,2,4,8 ..
	std::vector<int> addParityBits();
	std::vector<int> flipBit(std::vector<int>, size_t n); // 0->1 or 1->0
	std::vector<int> createErrVec(const std::vector <int> vec);
	void check_mistake(std::vector<int> vec, size_t word_length);
	int convertBinaryToDecimal(const std::vector<int> err_vec);
	bool checkLastParityBit(const std::vector<int> vec);
	bool vectorIsNull(const std::vector<int> vec);
	std::vector<int> decode(const std::vector<int> vec, size_t length);


};


