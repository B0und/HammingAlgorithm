#include "stdafx.h"
#include <string>
#include "HAMM.h"


HAMM::HAMM()
{
}


HAMM::~HAMM()
{
}
void HAMM::set_word()
{
	std::string input;
	
	std::cout << "Enter word : \n";
		std::getline(std::cin, input);

	std::vector<int> result;
	for (size_t i = 0; i < input.length(); ++i)
		result.push_back(input[i] - '0');

	word = result;
}

void HAMM::encrypt()
{
	code_word = addParityBits();
}
void HAMM::decipher()
{
	std::vector<int> decipher_result;
	check_mistake(code_word, word.size());
}
void HAMM::set_codeword_mistake(int pos)
{
	code_word = flipBit(code_word, pos);
}

void HAMM::show_word()
{
	for (auto i : word)
		std::cout << i << " ";
	std::cout << std::endl;
}
void HAMM::show_codeword()
{
	for (auto i : code_word)
		std::cout << i << " ";
	std::cout << std::endl;
}
void HAMM::show(std::vector<int> v)
{
	for (auto i : v)
		std::cout << i << " ";
	std::cout << std::endl;
}

/** encrypt helper functions */
std::vector<int> HAMM::addParityBits()
{
	std::vector<int> result_vec = word;
	size_t length = result_vec.size();
	std::vector<int> parVec;
	parVec = create_pow2_vec(length);
	////////////////////////
	// SET PARITY BITS TO 0
	for (size_t j = 0; j < parVec.size(); ++j)
	{
		int temp = parVec[j];
		//if (temp > 1) temp++;
		result_vec.insert(result_vec.begin() + temp - 1, 0);
	}
	/////////////////////////
	for (size_t i = 0; i < parVec.size(); ++i)
	{
		int parityValue, sum = 0, counter = 0;
		parityValue = parVec[i];
		// LOOP FOR FIRST PARITY BIT
		if (i == 0)
		{
			for (size_t j = i; j < result_vec.size(); j += 2)
			{
				sum += result_vec[j];
			}
			if (sum % 2 == 0) result_vec[i] = 0;
			else result_vec[i] = 1;
		}
		// LOOP FOR ALL OTHER PARITY BITS
		if (i != 0)
		{
			sum = 0;
			for (size_t k = parityValue - 1; k < result_vec.size(); ++k)
			{
				sum += result_vec[k];
				counter++;
				if (counter == parityValue)
				{
					k += parityValue;
					counter = 0;
				}
			}
			

			if (sum % 2 == 0) result_vec[parityValue - 1] = 0;
			else result_vec[parityValue - 1] = 1;
		}
	}
	// LAST PARITY BIT
	int cw_sum = 0; // maybe make sum static ?
	for (size_t i = 0; i < result_vec.size(); ++i) {
		cw_sum += result_vec[i];
	}
	if (cw_sum == 0 || cw_sum % 2 == 0) result_vec.push_back(0);
	else result_vec.push_back(1); 

	return result_vec;
}

std::vector<int> HAMM::flipBit(std::vector<int> vec, size_t n)
{
	if (n >= vec.size() || n<0)
	{
		std::cerr << "Cannot flip bit, number is out of scope \n";
		return vec;
	}

	if (vec[n] == 0) vec[n] = 1;
	else
		vec[n] = 0;
	return vec;
}


std::vector<int> HAMM::create_pow2_vec(size_t n)
{
	// parVec is a vector containing 2^x, x = 1,2,4,8 .. <= input vector length
	std::vector<int> parVec;
	for (auto i = 0; i <= n; ++i)
	{
		if (i && !(i & (i - 1)))
			parVec.push_back(i);
	}
	return parVec;
}

std::vector<int> HAMM::createErrVec(const std::vector <int> vec)
{
	const size_t word_length = word.size();
	std::vector<int> err_vec; // final product
	std::vector<int> parVec; // 1,2,4,8
	parVec = create_pow2_vec(word_length);
	std::vector<int> temp_vect;
	temp_vect = vec; // copy of main vector with error
	int parity_val_pos, sum, counter, temp_par_val;
	for (size_t i = 0; i < parVec.size(); ++i)
	{
		sum = 0, counter = 0;
		parity_val_pos = parVec[i]; // 1 2 4 8
		temp_par_val = temp_vect[parity_val_pos - 1];

		for (size_t k = parity_val_pos - 1; k < vec.size() - 1; ++k)
		{
			temp_vect[parity_val_pos - 1] = 0;
			sum += temp_vect[k];
			counter++;
			if (counter == parity_val_pos)
			{
				k += parity_val_pos;
				counter = 0;
			}
		}
		if (sum % 2 == 0) sum = 0;
		else sum = 1;
		if (temp_par_val == sum) err_vec.push_back(0);
		else err_vec.push_back(1);
	}
	return err_vec;
}

int HAMM::convertBinaryToDecimal(const std::vector<int> err_vec)
{
	int sum = 0;
	size_t i = 0;
	while (i < err_vec.size())
	{
		sum += err_vec[i] * pow(2, i);
		i++;
	}
	return sum - 1;
}

void HAMM::check_mistake(std::vector<int> vec, size_t word_length)
{
	std::vector<int> result;
	std::vector<int> err_vec = createErrVec(vec);
	std::cout << "err vec in check mistake : \n";
	show(err_vec);
	int err_pos_num = 0;
	bool isVecNull = vectorIsNull(err_vec); // true if null
	bool codewordParity = checkLastParityBit(vec); //  true if no parity changes in all bits

	if ((isVecNull == 1) && (1 == codewordParity)) std::cout << "No error occurred \n";
	else

		if ((isVecNull == 0) && (0 == codewordParity))
		{
			err_pos_num = convertBinaryToDecimal(err_vec);
			std::cout << "A single error occurred at position " << err_pos_num << std::endl;

			vec = flipBit(vec, err_pos_num);
			std::cout << "Correct codeword is : \n";
			show(vec);
			std::cout << "Word is : \n";
			result = decode(vec, word_length);
			show(result);
		}
		else

			if ((isVecNull == 0) && (1 == codewordParity)) std::cout << "Double error has occurred \n";
			else

				if ((isVecNull == 1) && (0 == codewordParity))
				{
					std::cout << "Error occured at last bit \n";
					vec = flipBit(vec, vec.size() - 1);
					std::cout << "Correct codeword is : \n";
					show(vec);
					std::cout << "Word is : \n";
					result = decode(vec, word_length);
					show(result);
				}
}

bool HAMM::checkLastParityBit(const std::vector<int> vec)
{
	int sum = 0;
	for (int i = 0; (unsigned)i< vec.size() - 1; ++i)
		sum += vec[i];
	int temp_par_b;
	if (sum == 0 || sum % 2 == 0) temp_par_b = 0;
	else temp_par_b = 1;

	if (temp_par_b == vec[vec.size() - 1]) return true;
	else return false;

}
bool HAMM::vectorIsNull(const std::vector<int> vec)
{
	int sum = 0; 
	for (size_t i = 0; i < vec.size(); ++i)
		sum += vec[i];
	if (sum == 0) return true;
	else return false;
}
std::vector<int> HAMM::decode(const std::vector<int> vec, size_t length)
{
	std::vector<int> parVec;
	std::vector<int> result;
	parVec = create_pow2_vec(length);
	for (size_t i = 0; i < parVec.size(); ++i)
		parVec[i]--;

	for (size_t i = 0; i < vec.size() - 1; ++i)
	{
		if (std::find(parVec.begin(), parVec.end(), i) != parVec.end())
		{
		}
		else
		{
			//cout << vec[i] << " ";
			result.push_back(vec[i]);
		}

	}

	std::cout << std::endl;
	return result;
}