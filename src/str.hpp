#ifndef STR_HPP
#define STR_HPP

#include <iostream>

#define START_SIZE 20
#define ADD_SIZE 10

// Utility class, could have used std::string

class Str
{
public:
	int length;
	int lengthMax;
	char *letters;
public:
	Str() : length(0), lengthMax(START_SIZE), letters(new char[lengthMax])
	{
	}
	Str(const char* txt)
	{
		length = 0;
		while (txt[length] != '\0' && length < 1000)
			length += 1;
		lengthMax = length;
		letters = new char[lengthMax];
		for (int i = 0; i < length; ++i)
			letters[i] = txt[i];
	}
	Str(const Str& str) : length(str.length), lengthMax(str.lengthMax), letters(new char[lengthMax])
	{
		for (int i = 0; i < length; ++i)
			letters[i] = str.letters[i];
	}
	explicit Str(int size) : length(0), lengthMax(size), letters(new char[lengthMax])
	{
	}
	~Str()
	{
		delete[] letters;
	}

	/* ######################################################    RESIZE    ###################################################### */

	void ResizeTo(int size)
	{
		if (size > lengthMax)
		{
			char* t = new char[size];
			for (int i = 0; i < length; ++i)
				t[i] = letters[i];
			delete[] letters;
			lengthMax = length = size;
			letters = t;
		}
		else
			length = size;
	}
	void ResizeTo_OnlyAlloc(int size)
	{
		if (size > lengthMax)
		{
			char* t = new char[size];
			for (int i = 0; i < length; ++i)
				t[i] = letters[i];
			delete[] letters;
			lengthMax = size;
			letters = t;
		}
	}
	void ResizeOf(int size)
	{
		size += length;
		if (size < 0)
			size = 0;
		ResizeTo(size);
	}
	void ResizeOf_OnlyAlloc(int size)
	{
		size += length;
		if (size < 0)
			size = 0;
		ResizeTo_OnlyAlloc(size);
	}
	void Remove(int size)
	{
		length -= size;
		if (length < 0)
			length = 0;
	}
	void Clear()
	{
		length = 0;
	}
	void RemoveAt(int pos)
	{
		if (length <= 0)
			return;
		if (pos < 0)
			pos = 0;
		else if (pos >= length)
			pos = length - 1;
		for (int i = pos + 1; i < length; ++i)
			letters[i - 1] = letters[i];
		length -= 1;
	}

	/* ######################################################    EQUALS    ###################################################### */

	bool EqualsCaseInsensitive(const Str &str)
	{
		if (length != str.length)
			return false;
		char caseGap = 'A' - 'a';
		for (int i = 0; i < length; ++i)
			if (letters[i] != str.letters[i] && ((letters[i] >= 'A' && letters[i] <= 'Z' && letters[i] - caseGap != str.letters[i]) || (letters[i] >= 'a' && letters[i] <= 'z' && letters[i] + caseGap != str.letters[i])))
				return false;
		return true;
	}
	bool operator==(const Str& str)
	{
		if (length != str.length)
			return false;
		for (int i = 0; i < lengthMax; ++i)
			if (letters[i] != str.letters[i])
				return false;
		return true;
	}
	bool operator==(const char* str)
	{
		int i = 0;
		while (str[i] != '\0')
		{
			if (i >= length || str[i] != letters[i])
				return false;
			i += 1;
		}
		if (i != length)
			return false;
		return true;
	}
	bool operator!=(const Str& str)
	{
		return !operator==(str);
	}
	bool operator!=(const char* str)
	{
		return !operator==(str);
	}
	bool operator>=(const Str& str)
	{
		int i = 0;
		while (i < str.length)
		{
			if (i >= length || str.letters[i] != letters[i])
				return false;
			i += 1;
		}
		if (i > length)
			return false;
		return true;
	}
	bool operator>=(const char* str)
	{
		int i = 0;
		while (str[i] != '\0')
		{
			if (i >= length || str[i] != letters[i])
				return false;
			i += 1;
		}
		if (i > length)
			return false;
		return true;
	}

	/* ######################################################    SET    ###################################################### */

	Str& operator=(const char* txt)
	{
		length = 0;
		while (txt[length] != '\0' && length < 1000)
			length += 1;
		if (length > lengthMax)
		{
			delete[] letters;
			lengthMax = length;
			letters = new char[lengthMax];
		}
		for (int i = 0; i < length; ++i)
			letters[i] = txt[i];
		return *this;
	}
	Str& operator=(const Str& str)
	{
		length = str.length;
		if (length > lengthMax)
		{
			delete[] letters;
			lengthMax = length;
			letters = new char[lengthMax];
		}
		for (int i = 0; i < length; ++i)
			letters[i] = str.letters[i];
		return *this;
	}

	/* ######################################################    ADD    ###################################################### */

	void AddCoordPath(int i)
	{
		ResizeOf_OnlyAlloc(2);
		letters[length] = '/';
		length += 1;
		letters[length] = '0' + i;
		length += 1;
	}
	void AddAt(char c, int pos)
	{
		ResizeOf_OnlyAlloc(1);
		if (pos < 0)
			pos = 0;
		else if (pos > length)
			pos = length - 1;
		for (int i = length; i > pos; --i)
			letters[i] = letters[i - 1];
		letters[pos] = c;
		length += 1;
	}
	Str& operator<<(const char& c)
	{
		ResizeOf(1);
		letters[length - 1] = c;
		return *this;
	}
	Str& operator<<(const char* txt)
	{
		int size = 0;
		while (txt[size] != '\0' && size < 1000)
			size += 1;
		ResizeOf_OnlyAlloc(size);
		for (int i = 0; i < size; ++i)
			letters[length + i] = txt[i];
		length += size;
		return *this;
	}
	Str& operator<<(const Str& str)
	{
		ResizeOf_OnlyAlloc(str.length);
		for (int i = 0; i < str.length; ++i)
			letters[length + i] = str.letters[i];
		length += str.length;
		return *this;
	}
	Str& operator<<(bool val)
	{
		if (val)
			operator=("true");
		else
			operator=("false");
		return *this;
	}
	Str& operator<<(int val)
	{
		ResizeOf_OnlyAlloc(10);
		if (val < 0)
		{
			letters[length] = '-';
			length += 1;
			val *= -1;
		}
		char buffer[12];
		int nb = 0;
		while (val > 0)
		{
			buffer[nb] = '0' + val % 10;
			val /= 10;
			nb += 1;
		}
		if (nb == 0)
		{
			letters[length] = '0';
			length += 1;
		}
		else
		{
			while (nb > 0)
			{
				nb -= 1;
				letters[length] = buffer[nb];
				length += 1;
			}
		}
		return *this;
	}
	Str& operator<<(unsigned int val)
	{
		ResizeOf_OnlyAlloc(10);
		char buffer[12];
		int nb = 0;
		while (val > 0)
		{
			buffer[nb] = '0' + val % 10;
			val /= 10;
			nb += 1;
		}
		if (nb == 0)
		{
			letters[length] = '0';
			length += 1;
		}
		else
		{
			while (nb > 0)
			{
				nb -= 1;
				letters[length] = buffer[nb];
				length += 1;
			}
		}
		return *this;
	}
	Str& operator<<(float val)
	{
		int accuracy = 6;
		ResizeOf_OnlyAlloc(10 + accuracy);
		if (val < 0)
		{
			letters[length] = '-';
			length += 1;
			val *= -1.0f;
		}
		int valint = (int)val;
		val -= (float)valint;
		char buffer[12];
		int nb = 0;
		while (valint > 0)
		{
			buffer[nb] = '0' + valint % 10;
			valint /= 10;
			nb += 1;
		}
		if (nb == 0)
		{
			letters[length] = '0';
			length += 1;
		}
		else
		{
			while (nb > 0)
			{
				nb -= 1;
				letters[length] = buffer[nb];
				length += 1;
			}
		}
		letters[length] = '.';
		length += 1;
		for (int i = 0; i < accuracy; ++i)
		{
			val *= 10.0f;
			valint = (int)val;
			val -= (float)valint;
			letters[length] = '0' + valint;
			length += 1;
		}
		return *this;
	}
	Str& operator<<(double val)
	{
		int accuracy = 12;
		ResizeOf_OnlyAlloc(10 + accuracy);
		if (val < 0)
		{
			letters[length] = '-';
			length += 1;
			val *= -1.0f;
		}
		int valint = (int)val;
		val -= (double)valint;
		char buffer[12];
		int nb = 0;
		while (valint > 0)
		{
			buffer[nb] = '0' + valint % 10;
			valint /= 10;
			nb += 1;
		}
		if (nb == 0)
		{
			letters[length] = '0';
			length += 1;
		}
		else
		{
			while (nb > 0)
			{
				nb -= 1;
				letters[length] = buffer[nb];
				length += 1;
			}
		}
		letters[length] = '.';
		length += 1;
		for (int i = 0; i < accuracy; ++i)
		{
			val *= 10.0f;
			valint = (int)val;
			val -= (double)valint;
			letters[length] = '0' + valint;
			length += 1;
		}
		return *this;
	}

	/* ######################################################    EXTRACT    ###################################################### */

	void Extract(Str& str)
	{
		str = *this;
	}
	bool Extract(bool &valb)
	{
		DeleteFrontBackSpaces();
		if (length >= 4)
		{
			if ((letters[0] == 't' || letters[0] == 'T') && (letters[1] == 'r' || letters[1] == 'R') && (letters[2] == 'u' || letters[2] == 'U') && (letters[3] == 'e' || letters[3] == 'E'))
			{
				valb = true;
				return true;
			}
			if (length >= 5)
			{
				if ((letters[0] == 'f' || letters[0] == 'F') && (letters[1] == 'a' || letters[1] == 'A') && (letters[2] == 'l' || letters[2] == 'L') && (letters[3] == 's' || letters[3] == 'S') && (letters[4] == 'e' || letters[4] == 'E'))
				{
					valb = false;
					return true;
				}
			}
		}
		int pos = 0;
		while (pos < length && (letters[pos] < '0' || letters[pos] > '9'))
			pos += 1;
		if (pos >= length)
			return false;
		int val = 0;
		bool negate = false;
		if (pos - 1 >= 0 && letters[pos - 1] == '-')
			negate = true;
		while (pos < length && letters[pos] >= '0' && letters[pos] <= '9')
		{
			val *= 10;
			val += letters[pos] - '0';
			pos += 1;
		}
		if (negate)
			val *= -1;
		if (val == 0)
		{
			valb = false;
			return true;
		}
		else
		{
			valb = true;
			return true;
		}
	}
	bool Extract(int &val)
	{
		int pos = 0;
		while (pos < length && (letters[pos] < '0' || letters[pos] > '9'))
			pos += 1;
		if (pos >= length)
			return false;
		val = 0;
		bool negate = false;
		if (pos - 1 >= 0 && letters[pos - 1] == '-')
			negate = true;
		while (pos < length && letters[pos] >= '0' && letters[pos] <= '9')
		{
			val *= 10;
			val += letters[pos] - '0';
			pos += 1;
		}
		if (negate)
			val *= -1;
		return true;
	}
	bool Extract(unsigned int &val)
	{
		int pos = 0;
		while (pos < length && (letters[pos] < '0' || letters[pos] > '9'))
			pos += 1;
		if (pos >= length)
			return false;
		val = 0;
		while (pos < length && letters[pos] >= '0' && letters[pos] <= '9')
		{
			val *= 10;
			val += letters[pos] - '0';
			pos += 1;
		}
		return true;
	}
	bool Extract(double &val)
	{
		int pos = 0;
		while (pos < length && (letters[pos] < '0' || letters[pos] > '9') && letters[pos] != '.')
			pos += 1;
		if (pos >= length)
			return false;
		val = 0.0f;
		bool negate = false;
		if (pos - 1 >= 0 && letters[pos - 1] == '-')
			negate = true;
		while (pos < length && letters[pos] != '.' && letters[pos] >= '0' && letters[pos] <= '9')
		{
			val *= 10;
			val += letters[pos] - '0';
			pos += 1;
		}
		if (pos < length && letters[pos] == '.')
		{
			pos += 1;
			double multiplier = 0.1f;
			while (pos < length && letters[pos] >= '0' && letters[pos] <= '9')
			{
				val += (double)(letters[pos] - '0') * multiplier;
				multiplier *= 0.1f;
				pos += 1;
			}
		}
		if (negate)
			val *= -1.0f;
		return true;
	}
	bool Extract(float &val)
	{
		int pos = 0;
		while (pos < length && (letters[pos] < '0' || letters[pos] > '9') && letters[pos] != '.')
			pos += 1;
		if (pos >= length)
			return false;
		val = 0.0f;
		bool negate = false;
		if (pos - 1 >= 0 && letters[pos - 1] == '-')
			negate = true;
		while (pos < length && letters[pos] != '.' && letters[pos] >= '0' && letters[pos] <= '9')
		{
			val *= 10;
			val += letters[pos] - '0';
			pos += 1;
		}
		if (pos < length && letters[pos] == '.')
		{
			pos += 1;
			float multiplier = 0.1f;
			while (pos < length && letters[pos] >= '0' && letters[pos] <= '9')
			{
				val += (float)(letters[pos] - '0') * multiplier;
				multiplier *= 0.1f;
				pos += 1;
			}
		}
		if (negate)
			val *= -1.0f;
		return true;
	}
	bool ExtractInt(int &val)
	{
		int pos = 0;
		while (pos < length && (letters[pos] < '0' || letters[pos] > '9'))
			pos += 1;
		if (pos >= length)
			return false;
		val = 0;
		bool negate = false;
		if (pos - 1 >= 0 && letters[pos - 1] == '-')
			negate = true;
		while (pos < length && letters[pos] >= '0' && letters[pos] <= '9')
		{
			val *= 10;
			val += letters[pos] - '0';
			pos += 1;
		}
		if (negate)
			val *= -1;
		return true;
	}
	bool ExtractInt(int &val, int &pos)
	{
		while (pos < length && (letters[pos] < '0' || letters[pos] > '9'))
			pos += 1;
		if (pos >= length)
			return false;
		val = 0;
		bool negate = false;
		if (pos - 1 >= 0 && letters[pos - 1] == '-')
			negate = true;
		while (pos < length && letters[pos] >= '0' && letters[pos] <= '9')
		{
			val *= 10;
			val += letters[pos] - '0';
			pos += 1;
		}
		if (negate)
			val *= -1;
		return true;
	}
	bool ExtractFloat(float &val)
	{
		int pos = 0;
		while (pos < length && (letters[pos] < '0' || letters[pos] > '9') && letters[pos] != '.')
			pos += 1;
		if (pos >= length)
			return false;
		val = 0.0f;
		bool negate = false;
		if (pos - 1 >= 0 && letters[pos - 1] == '-')
			negate = true;
		while (pos < length && letters[pos] != '.' && letters[pos] >= '0' && letters[pos] <= '9')
		{
			val *= 10;
			val += letters[pos] - '0';
			pos += 1;
		}
		if (pos < length && letters[pos] == '.')
		{
			pos += 1;
			float multiplier = 0.1f;
			while (pos < length && letters[pos] >= '0' && letters[pos] <= '9')
			{
				val += (float)(letters[pos] - '0') * multiplier;
				multiplier *= 0.1f;
				pos += 1;
			}
		}
		if (negate)
			val *= -1.0f;
		return true;
	}
	bool ExtractFloat(float &val, int &pos)
	{
		while (pos < length && (letters[pos] < '0' || letters[pos] > '9') && letters[pos] != '.')
			pos += 1;
		if (pos >= length)
			return false;
		val = 0.0f;
		bool negate = false;
		if (pos - 1 >= 0 && letters[pos - 1] == '-')
			negate = true;
		while (pos < length && letters[pos] != '.' && letters[pos] >= '0' && letters[pos] <= '9')
		{
			val *= 10;
			val += letters[pos] - '0';
			pos += 1;
		}
		if (pos < length && letters[pos] == '.')
		{
			pos += 1;
			float multiplier = 0.1f;
			while (pos < length && letters[pos] >= '0' && letters[pos] <= '9')
			{
				val += (float)(letters[pos] - '0') * multiplier;
				multiplier *= 0.1f;
				pos += 1;
			}
		}
		if (negate)
			val *= -1.0f;
		return true;
	}

	/* ######################################################    MISC    ###################################################### */

	void Lowercase()
	{
		char gap = 'a' - 'A';
		for (int i = 0; i < length; ++i)
			if (letters[i] >= 'A' && letters[i] <= 'Z')
				letters[i] += gap;
	}
	void Uppercase()
	{
		char gap = 'A' - 'a';
		for (int i = 0; i < length; ++i)
			if (letters[i] >= 'a' && letters[i] <= 'z')
				letters[i] += gap;
	}
	bool AlphaNum()
	{
		for (int i = 0; i < length; ++i)
			if ((letters[i] < 'a' || letters[i] > 'z') && (letters[i] < 'A' || letters[i] > 'Z') && letters[i] != '_')
				return false;
		return true;
	}
	void DeleteFrontBackSpaces()
	{
		while (length > 0 && letters[0] == ' ')
		{
			length -= 1;
			for (int i = 0; i < length; ++i)
				letters[i] = letters[i + 1];
		}
		while (length > 0 && letters[length - 1] == ' ')
		{
			length -= 1;
		}
	}
	void AddEndStringChar()
	{
		if (length < 1 || letters[length - 1] != '\0')
			operator<<('\0');
	}
	void RemoveEndStringChar()
	{
		if (length > 0 && letters[length - 1] == '\0')
			Remove(1);
	}
	bool KeepLettersAfter(char c)
	{
		int count = 0;
		while (count < length && letters[count] != c)
			++count;
		if (count < length)
		{
			++count;
			length -= count;
			for (int i = 0; i < length; ++i)
				letters[i] = letters[i + count];
			return true;
		}
		return false;
	}
	int Count(char c)
	{
		int sum = 0;
		for (int i = 0; i < length; ++i)
		{
			if (letters[i] == c)
				sum += 1;
		}
		return sum;
	}
	void Replace(char any, char by)
	{
		for (int i = 0; i < length; ++i)
		{
			if (letters[i] == any)
				letters[i] = by;
		}
	}
	char* Get()
	{
		return letters;
	}
	friend std::ostream& operator<<(std::ostream& os, const Str& str)
	{
		for (int i = 0; i < str.length; ++i)
			os << str.letters[i];
		return os;
	}
};


#endif