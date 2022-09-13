#ifndef INCLUDE_GUARD_SANAEUTIL_H
#define INCLUDE_GUARD_SANAEUTIL_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <stdexcept>
#include <exception>
#ifndef INCLUDE_GUARD_SANAENET_H
	#if __has_include("winsock2.h")
		#include<winsock2.h>
	#endif
#endif
#if __has_include("chrono")
	#include <chrono>
#endif
#if __has_include("windows.h")
	#include <windows.h>
#endif
#include "SanaeBase.h"

namespace sanae {
	/*-----------------------------------------------
	* Project:SanaeProject-UsefulHeadersProject
	* Dev:SanaeProject
	* Function:
	This header have many utility programs. 
	*Copyright 2021 SanaeProject.ALL Rights Reserved.
	-----------------------------------------------*/
	namespace util {
		/*sanae::util::arraylist<int, 入れる個数> test = {値...};*/
		template<typename T,int C=0> class arraylist:SANAE_ARRAY_BASE<T> {
		private:
			T* data=NULL;
			int datalen = 0;
		public:
			/*CONSTRUCTOR*/
			~arraylist() {
				this->clear();
			}
			arraylist(const arraylist& d) {
				this->clear();
				if (!arraylist::_calloc(&data, d.datalen, false))arraylist::mem_error();
				for (int i = 0; i < d.datalen; i++) {
					this->data[i] = d.data[i];
				}
			}
			arraylist(...) {
				if (datalen != 0) {
					arraylist::sfree(data);
				}
				if (C == 0) {
					arraylist::sfree(data);
				}
				else {
					va_list ap;
					va_start(ap, C);
					va_arg(ap, T);
					if (!arraylist::_calloc(&data, C, false))arraylist::mem_error();
					for (int i = 0; i < C; i++) {
						*(data + i) = va_arg(ap, T);
					}
					va_end(ap);
					datalen = C;
				}
			}
			/*OPERATOR*/
			T& operator [](unsigned int t) {
				if ((unsigned int)datalen <= t) {
					arraylist::range_error();
				}
				return *(data + t);
			}
			void operator +=(T d) { this->add(d); }
			/*FUNCTION*/
			void reverse() {arraylist::_reverse(&data);}
			int to_array(T* arr, size_t count) {
				if ((this->len()) >= count) {
					for (int i = 0; i < (this->len()); i++) {
						*(arr + i) = *(data + i);
					}
					return 1;
				}
				return -1;
			}
			void del(int number) {
				if (number < datalen) {
					datalen -= 1;
					T* d1 = NULL;
					if (!arraylist::_calloc(&d1, datalen, false))arraylist::mem_error();
					for (int i = 0, i2 = 0; i2 < datalen; i++) {
						if (i == number) {
							continue;
						}
						*(d1 + i2) = *(data + i);
						i2 += 1;
					}
					arraylist::sfree(data);
					data = NULL;
					if (!arraylist::_calloc(&data, datalen, false))arraylist::mem_error();
					for (int i = 0; i < datalen; i++) {
						*(data + i) = *(d1 + i);
					}
				}
			}
			void add(T d) {
				if (datalen == 0) {
					datalen += 1;
					data = NULL;
					if (!arraylist::_calloc(&data, datalen, false))arraylist::mem_error();
					data[datalen - 1] = d;
				}
				else {
					T* d_c = NULL;
					if (!arraylist::_calloc(&d_c, datalen, false))arraylist::mem_error();
					for (int i = 0; i < datalen; i++) {
						*(d_c + i) = *(data + i);
					}
					arraylist::sfree(data);
					datalen += 1;
					data = NULL;
					if (!arraylist::_calloc(&data, datalen, false))arraylist::mem_error();
					for (int i = 0; i < datalen - 1; i++) {
						*(data + i) = *(d_c + i);
					}
					*(data + (datalen - 1)) = d;
					arraylist::sfree(d_c);
				}
			}
			void clear() {
				arraylist::sfree(data);
				datalen = 0;
			}
			/*引数を検索して発見した場所の配列番号を返します。*/
			int find(T t) {
				for (unsigned int i = 0; i < (this->len()); i++) {
					if (t == this->data[i]) {
						return i;
					}
				}
				return -1;
			}
			/*要素数を返します。*/
			size_t len() {
				return this->datalen;
			}
			/*配列を第一引数個確保し0または第二引数で埋めます。*/
			void secured(int l, T d = NULL) {
				for (int i = 0; i < l; i++) {
					this->add(d);
				}
			}
			void into(T** d,unsigned int count) {
				arraylist::_ARRAY_COPY(&data,d,count);
			}
		};
		/*渡された配列の中に何個指定した値があるか返します*/
		template<class T>int count(T** array, int len, T to) {
			int c = 0;
			for (int i = 0; i < len; i++) {
				if ((*array)[i] == to) {
					c += 1;
				}
			}
			return c;
		}
		/*2種類の値を格納できます。*/
		template<class T1, class T2>class pair{
		private:
			T1 t1;
			T2 t2;
		public:
			bool operator ==(sanae::util::pair<T1, T2> t) {
				sanae::util::pair<T1, T2>* te = &t;
				if (te->first() == this->first() && te->second() == this->second())return true;
				return false;
			}
			void operator =(sanae::util::pair<T1, T2> t) {
				sanae::util::pair<T1, T2>* p = &t;
				this->t1 = p->first();
				this->t2 = p->second();
			}
			pair(T1 d1, T2 d2) :t1(d1), t2(d2) {};
			pair() { t1 = NULL; t2 = NULL; };
			void set(T1 d1, T2 d2) { t1 = d1; t2 = d2; }
			T1 first() { return t1; }
			T2 second() { return t2; }
		};
#if __has_include("windows.h")
		enum class colors { black = 0x0000, blue = 0x0001, green = 0x0002, cyan = 0x0003, red = 0x0004, pink = 0x0005, yellow = 0x0006, white = 0x0007 };
		template<typename T = colors>
		bool set_console_color(T color = colors::white, T backcolor = colors::black,bool shine = false,bool back_shine=false) {
			void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
			unsigned short word = (char)color;
			word |= shine ? 0x0008 : 0x0000;
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo((HANDLE)handle, &csbi);
			word |= (char)((int)backcolor * 16);
			word |= back_shine ? 0x0080 : 0x0000;
			return SetConsoleTextAttribute((HANDLE)handle, (WORD)word);
		}
#endif
#if __has_include("chrono")
		typedef long long LLONG;
		namespace SC = std::chrono;
		enum class CLOCKTYPES{ second, millisecond, microsecond , min};
		class clock {
		private:
			std::chrono::system_clock::time_point	s_point		=	SC::system_clock::now();
			std::chrono::seconds					s_second	=	SC::duration_cast<SC::seconds>(s_point.time_since_epoch());
			std::chrono::milliseconds				s_milli		=	SC::duration_cast<SC::milliseconds>(s_point.time_since_epoch());
			std::chrono::microseconds				s_micro		=	SC::duration_cast<SC::microseconds>(s_point.time_since_epoch());
			std::chrono::minutes					s_min		=	SC::duration_cast<SC::minutes>(s_point.time_since_epoch());
			CLOCKTYPES								unit_save	=	CLOCKTYPES::millisecond;
		public:
			clock() {}
			clock(const clock& c) {
				switch (c.unit_save) {
				case CLOCKTYPES::second:
					s_second	=	c.s_second;
					break;
				case CLOCKTYPES::millisecond:
					s_milli		=	c.s_milli;
					break;
				case CLOCKTYPES::microsecond:
					s_micro		=	c.s_micro;
					break;
				case CLOCKTYPES::min:
					s_min		=	c.s_min;
					break;
				}
			}
			void start(CLOCKTYPES unit = CLOCKTYPES::millisecond){
				unit_save	=	unit;
				s_point		=	SC::system_clock::now();
				switch (unit) {
				case CLOCKTYPES::second:
					s_second	=	SC::duration_cast<SC::seconds>(s_point.time_since_epoch());
					break;
				case CLOCKTYPES::millisecond:
					s_milli		=	SC::duration_cast<SC::milliseconds>(s_point.time_since_epoch());
					break;
				case CLOCKTYPES::microsecond:
					s_micro		=	SC::duration_cast<SC::microseconds>(s_point.time_since_epoch());
					break;
				case CLOCKTYPES::min:
					s_min		=	SC::duration_cast<SC::minutes>(s_point.time_since_epoch());
					break;
				}
			}
			LLONG end() {
				SC::system_clock::time_point e_point = SC::system_clock::now();
				switch (unit_save) {
				case CLOCKTYPES::second:
					SC::seconds e_second		= SC::duration_cast<SC::seconds>(e_point.time_since_epoch());
					return (e_second - s_second).count();
				case CLOCKTYPES::millisecond:
					SC::milliseconds e_milli	= SC::duration_cast<SC::milliseconds>(e_point.time_since_epoch());
					return (e_milli - s_milli).count();
				case CLOCKTYPES::microsecond:
					SC::microseconds e_micro	= SC::duration_cast<SC::microseconds>(e_point.time_since_epoch());
					return (e_micro - s_micro).count();
				case CLOCKTYPES::min:
					SC::minutes e_min			= SC::duration_cast<SC::minutes>(e_point.time_since_epoch());
					return (LLONG)(e_min - s_min).count();
				}
				return (LLONG)-1;
			}
		};
#endif
		/*
		入力されたキーとデータを格納します。
		get_data(キー);でデータを取得します。
		*/
		template<typename K, typename D>class map {
		private:
			std::vector<K> keys;
			std::vector<D> datas;
			/*一時保存用変数*/
			K key_cp;
			D data_cp;
		public:
			map(K key, D data) :key_cp(key),data_cp(data){
				keys.clear();
				datas.clear();
				keys.push_back(key_cp);
				datas.push_back(data_cp);
			}
			map() {}
			~map() {
				this->clear();
			}
			//要素数を返します
			size_t len() { return keys.len(); }
			//見つかった場合true 見つからなかった場合false
			bool find(D data) {
				auto found = std::find(datas.cbegin(),datas.cend(),data);
				if (found!=datas.cend()) {
					return true;
				}
				return false;
			}
			/*データを追加します。
			返り値:成功:成功した配列番号
			失敗:-1
			*/
			int add(K key,D data) {
				auto found = std::find(keys.cbegin(), keys.cend(), key);
				if (found == keys.cend()) {
					keys.push_back(key);
					datas.push_back(data);
					return keys.size() - 1;
				}else {
					return -1;
				}
			}
			//データが見つからない場合NULLを返します。
			D get_data(K key) {
				auto found = std::find(keys.cbegin(), keys.cend(), key);
				if (found == keys.cend()) {
					return NULL;
				}
				int number = 0;
				for (int i = 0; i < keys.size();i++) {
					if (key==keys[i]) {
						number = i;
						break;
					}
				}
				return datas[number];
			}
			//keyのデータを消去します。
			void del_data(K key) {
				auto found = std::find(keys.cbegin(), keys.cend(), key);
				if (found != keys.cend()) {
					int number = 0;
					for (int i = 0; i < keys.size(); i++) {
						if (key == keys[i]) {
							number = i;
							break;
						}
					}
					keys.erase(keys.begin()+number);
					datas.erase(datas.begin()+number);
				}
			}
			//データをクリアします。
			void clear() {
				keys.clear();
				datas.clear();
			}
		};
#if _HAS_CXX17
		//補数を利用したバイナリで表示、計算します。(int)
		template<typename T = int>
		class binary {
			static inline std::vector<bool> data;
		private:
			bool minus_binary = false;
			sanae::str data_binary = "";
			//complement1(Inversion)
			void complement_1(std::vector<bool>* d) {
				for (T i = 0; i < (T)(d->size()); i++)(*d)[i] = (*d)[i] ? false : true;
			}
			//complement2(plus1)
			void complement_2(std::vector<bool>* d) {
				bool advance = true;
				for (int i = (*d).size() - 1; i >= 0; i--) {
					if ((*d)[i] && advance) {
						(*d)[i] = false;
						advance = true;
					}
					else if (advance) {
						(*d)[i] = true;
						advance = false;
						break;
					}
				}
			}
			//Convert decimal to binary
			void to_binary(T d, std::vector<bool>* da = &data) {
				minus_binary = false;
				if (d < 0) {
					minus_binary = true;
					d *= -1;
				}
				else {
					minus_binary = false;
				}
				while (1) {
					(*da).push_back(d % 2);
					d = d / 2;
					if (d == 1 || d == 0) {
						(*da).push_back(d);
						break;
					}
				}
				for (T i = (T)da->size(); i < 3; i++) {
					(*da).push_back(0);
				}
				(*da).push_back(0);
				std::reverse((*da).begin(), (*da).end());
				if (minus_binary) {
					complement_1(da);
					complement_2(da);
				}
			}
			//Convert binary to decimal
			T to_decimal(std::vector<bool>* d = &data) {
				bool minus = false;
				if ((*d)[0]) {
					complement_1(d);
					complement_2(d);
					minus = true;
				}
				T data_decimal = 0;
				for (T i = d->size() - 1, count = 0; i >= 0; i--, count++) {
					T plus_data = 0;
					if ((*d)[i]) {
						if (count == 0) {
							plus_data = 1;
						}
						else {
							plus_data = 1;
							for (T i = 0; i < count; i++) {
								plus_data *= 2;
							}
						}
						data_decimal += plus_data;
					}
				}
				if (minus) {
					data_decimal *= -1;
					complement_1(d);
					complement_2(d);
				}
				return data_decimal;
			}
		public:
			//Pass binary value.
			const char* binary_num() {
				data_binary = "";
				for (T i : data) {
					data_binary.addint(i);
				}
				return data_binary.c_str();
			}
			//Operator
			bool operator [](T count) {
				if ((data.size() - 1) < count) {
					throw std::runtime_error("Tried to access of out value of range.");
				}
				return data[count];
			}
			void operator =(const binary& t) {
				data.erase(data.begin(), data.end());
				for (T i : t.data) {
					data.push_back(i);
				}
				if (t.minus_binary) {
					complement_1(&data);
					complement_2(&data);
				}
			}
			//Four arithmetic operations
			T operator +(T d) {
				d += this->decimal();
				return d;
			}
			void operator+=(T d) {
				d += this->decimal();
				data.erase(data.begin(), data.end());
				to_binary(d, &data);
			}
			T operator -(T d) {
				d -= this->decimal();
				return d;
			}
			void operator -=(T d) {
				d -= this->decimal();
				data.erase(data.begin(), data.end());
				to_binary(d, &data);
			}
			T operator *(T d) {
				d *= this->decimal();
				return d;
			}
			void operator *=(T d) {
				d *= this->decimal();
				data.erase(data.begin(), data.end());
				to_binary(d, &data);
			}
			T operator /(T d) {
				d /= this->decimal();
				return d;
			}
			void operator /=(T d) {
				d /= this->decimal();
				data.erase(data.begin(), data.end());
				to_binary(d, &data);
			}
			//Constructor
			binary(T d) {
				to_binary(d);
			}
			//Copyconstructor
			binary(const binary& t) {
				data.erase(data.begin(), data.end());
				for (T i : t.data) {
					data.push_back(i);
				}
				if (t.minus_binary) {
					complement_1(&data);
					complement_2(&data);
				}
			}
			void inversion() {
				complement_1(&data);
			}
			//Destructor
			~binary() {
				minus_binary = false;
				data.clear();
			}
			//Pass decimal value.
			T decimal() {
				return to_decimal();
			}
		};
#endif
#if defined(INCLUDE_GUARD_SANAESTR_H) && __has_include("windows.h")
		sanae::str GNT = "";	
		const char* GET_NOW_TIME() {
			GNT = "";
			time_t t = 0;
			struct tm local = {};
			t = time(NULL);
			localtime_s(&local, &t);
			int second = local.tm_sec;
			int min = local.tm_min;
			int hour = local.tm_hour;
			GNT.addint(hour);
			GNT.addchr(':');
			GNT.addint(min);
			GNT.addchr(':');
			GNT.addint(second);
			return GNT.c_str();
		}
		class file:SANAE_BASE {
		private:
			FILE*		fpointer	=	NULL;
			sanae::str	fname		=	"";
			sanae::str	text		=	"";
		public:
			file(const char* filename) {
				fopen_s(&fpointer, filename, "a");
				fclose(fpointer);
				fname.add(filename);
			}
			void add(const char* text) {
				fopen_s(&fpointer, fname.c_str(), "a");
				fwrite(text, sizeof(*text), strlen(text), fpointer);
				fclose(fpointer);
			}
			void write(const char* text) {
				fopen_s(&fpointer,fname.c_str(),"w");
				fwrite(text,sizeof(*text),strlen(text),fpointer);
				fclose(fpointer);
			}
			const char* read() {
				try {
					fopen_s(&fpointer, fname.c_str(), "r");
				}
				catch (std::exception) {
					SANAE_BASE::runtime_error("Not here the file.");
				}
				int	c=0;
				text = "";
				while (EOF != (c = fgetc(fpointer)))
				{
					text.addchr(c);
				}
				fclose(fpointer);
				return text.c_str();
			}
		};
#endif
	}
	/*random*/
	namespace random {
		bool seedset = false;
		/*乱数のシード値を設定する*/
		void setseed(unsigned long seed = (unsigned)time(0UL)) {
			srand(seed);
			seedset = true;
		}
		/*乱数シード値設定を初期化*/
		void seedreset() { seedset = false; };
		/*
		乱数生成:random(最低値,最高値,任意シード値);
		最低値～最高値までの値が出されます。
		任意シード値を使用すると次のシード値を入れても書き換えられないため、
		シード値を設定した場合はseedreset()関数をご使用ください。
		*/
		int random(int min, int max, unsigned long seed = 0) {
			if (seed != 0 && !seedset)setseed(seed);
			if (!seedset)setseed();
			max += 1;
			if (min > max || min == max) {
				return -1;
			}
			int ran = rand() % max;
			if (ran < min) {
				ran = ran + min;
				if (ran > max) {
					ran = ran - (ran - max);
				}
			}
			return ran;
		}
	}
}
#endif