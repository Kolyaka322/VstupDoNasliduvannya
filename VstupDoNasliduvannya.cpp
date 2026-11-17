#include <iostream>
#include <string>
#include <cstring>

class Pet {
protected:
    std::string name;
    std::string characteristics;
public:
    Pet(const std::string& name, const std::string& characteristics)
        : name(name), characteristics(characteristics) {
    }
    virtual ~Pet() {}
    virtual void display() const = 0;
};

class Dog : public Pet {
public:
    Dog(const std::string& name, const std::string& characteristics)
        : Pet(name, characteristics) {
    }
    void display() const override {
        std::cout << "Dog: " << name << ", Characteristics: " << characteristics << std::endl;
    }
};

class Cat : public Pet {
public:
    Cat(const std::string& name, const std::string& characteristics)
        : Pet(name, characteristics) {
    }
    void display() const override {
        std::cout << "Cat: " << name << ", Characteristics: " << characteristics << std::endl;
    }
};

class Parrot : public Pet {
public:
    Parrot(const std::string& name, const std::string& characteristics)
        : Pet(name, characteristics) {
    }
    void display() const override {
        std::cout << "Parrot: " << name << ", Characteristics: " << characteristics << std::endl;
    }
};

class String {
protected:
    char* data;
    size_t len;
public:
    String() : data(new char[1] {'\0'}), len(0) {}

    String(const char* cstr) {
        if (cstr) {
            len = std::strlen(cstr);
            data = new char[len + 1];
            std::strcpy(data, cstr);
        }
        else {
            data = new char[1] {'\0'};
            len = 0;
        }
    }

    String(const String& other) {
        len = other.len;
        data = new char[len + 1];
        std::strcpy(data, other.data);
    }

    virtual ~String() {
        delete[] data;
    }

    String& operator=(const String& other) {
        if (this == &other) return *this;
        delete[] data;
        len = other.len;
        data = new char[len + 1];
        std::strcpy(data, other.data);
        return *this;
    }

    size_t length() const {
        return len;
    }

    void clear() {
        delete[] data;
        data = new char[1] {'\0'};
        len = 0;
    }

    String operator+(const String& other) const {
        size_t new_len = len + other.len;
        char* temp = new char[new_len + 1];
        std::strcpy(temp, data);
        std::strcat(temp, other.data);
        String res(temp);
        delete[] temp;
        return res;
    }

    String& operator+=(const String& other) {
        size_t new_len = len + other.len;
        char* temp = new char[new_len + 1];
        std::strcpy(temp, data);
        std::strcat(temp, other.data);
        delete[] data;
        data = temp;
        len = new_len;
        return *this;
    }

    bool operator==(const String& other) const {
        return std::strcmp(data, other.data) == 0;
    }

    bool operator!=(const String& other) const {
        return !(*this == other);
    }

    const char* c_str() const { return data; }
};

class BitString : public String {
private:
    bool isValid(const char* cstr) const {
        if (!cstr || cstr[0] == '\0') return false;
        for (size_t i = 0; cstr[i] != '\0'; ++i) {
            if (cstr[i] != '0' && cstr[i] != '1') return false;
        }
        return true;
    }

    long long toLongLong() const {
        if (len == 0) return 0;
        long long val = 0;
        bool isNegative = (data[0] == '1');
        if (!isNegative) {
            for (size_t i = 0; i < len; ++i) {
                val = (val << 1) + (data[i] - '0');
            }
        }
        else {
            BitString temp(*this);
            temp.changeSign();
            for (size_t i = 0; i < temp.len; ++i) {
                val = (val << 1) + (temp.data[i] - '0');
            }
            val = -val;
        }
        return val;
    }

    static BitString fromLongLong(long long val) {
        if (val == 0) return BitString("0");
        std::string s = "";
        long long temp = val;
        bool neg = (val < 0);
        if (neg) temp = -temp;
        while (temp > 0) {
            s = (temp % 2 == 0 ? "0" : "1") + s;
            temp /= 2;
        }
        if (neg) {
            s = "0" + s;
            BitString bs(s.c_str());
            bs.changeSign();
            return bs;
        }
        else {
            if (s[0] == '1') s = "0" + s;
            return BitString(s.c_str());
        }
    }

public:
    BitString() : String("0") {}

    BitString(const char* cstr) {
        if (isValid(cstr)) {
            len = std::strlen(cstr);
            data = new char[len + 1];
            std::strcpy(data, cstr);
        }
        else {
            data = new char[1] {'\0'};
            len = 0;
        }
    }

    BitString(const BitString& other) : String(other) {}

    BitString& operator=(const BitString& other) {
        if (this == &other) return *this;
        String::operator=(other);
        return *this;
    }

    ~BitString() {}

    void changeSign() {
        if (len == 0) return;
        for (size_t i = 0; i < len; ++i) {
            data[i] = (data[i] == '0') ? '1' : '0';
        }
        int carry = 1;
        for (int i = (int)len - 1; i >= 0; --i) {
            if (carry == 0) break;
            if (data[i] == '0') {
                data[i] = '1';
                carry = 0;
            }
            else {
                data[i] = '0';
                carry = 1;
            }
        }
        if (carry == 1) {
            char* temp = new char[len + 2];
            temp[0] = '1';
            std::strcpy(temp + 1, data);
            delete[] data;
            data = temp;
            len++;
        }
    }

    BitString operator+(const BitString& other) const {
        long long v1 = this->toLongLong();
        long long v2 = other.toLongLong();
        return fromLongLong(v1 + v2);
    }

    BitString& operator+=(const BitString& other) {
        *this = *this + other;
        return *this;
    }

    bool operator==(const BitString& other) const {
        return toLongLong() == other.toLongLong();
    }

    bool operator!=(const BitString& other) const {
        return !(*this == other);
    }
};

int main() {
    system("chcp 1251>null");
    Dog dog("Rex", "Friendly, active, guard dog");
    Cat cat("Murka", "Independent, calm, mouser");
    Parrot parrot("Kesha", "Talkative, colorful, active");

    dog.display();
    cat.display();
    parrot.display();

    BitString bs1("1010");
    BitString bs2("0111");
    BitString bs3("1021");

    std::cout << "bs3 length (should be 0 due to invalid char): " << bs3.length() << std::endl;

    BitString bs4 = bs1 + bs2;
    bs1.changeSign();

    return 0;
}
