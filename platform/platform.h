#pragma once

#ifdef PLATFORM_EXPORTS
#define PLATFORM_API __declspec(dllexport)
#else
#define PLATFORM_API __declspec(dllimport)
#endif

#include <random>
#include <vector>

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

constexpr char UP_FLAG{ 0b00000001 };
constexpr char DOWN_FLAG{ 0b00000010 };
constexpr char LEFT_FLAG{ 0b00000100 };
constexpr char RIGHT_FLAG{ 0b00001000 };

enum class dirs { up = 0, down = 1, left = 2, right = 3, stop = 4 };
enum class creatures {
	evil1 = 0, evil2 = 1, evil3 = 2, evil4 = 3,
	evil5 = 4, evil6 = 5, hero = 6
};
enum class fields { field1 = 0, field2 = 1, field3 = 2, field4 = 3, field5 = 4 };

namespace gamedll
{
	class PLATFORM_API RANDENGINE
	{
		private:
			std::random_device rand_device{};
			std::seed_seq* seeder{ nullptr };
			std::vector<int> vSeeds;
			std::mt19937* twister{ nullptr };

		public:

			RANDENGINE();
			~RANDENGINE();

			int operator() (int min, int max);
	};

	class PLATFORM_API ATOM
	{
		protected:
			float width {};
			float height{};

		public:
			float x{ 0 };
			float y{ 0 };
			float ex{ 0 };
			float ey{ 0 };

			ATOM(float _x = 0, float _y = 0, float _width = 1, float _height = 1);
			virtual ~ATOM() {};

			float GetWidth() const;
			float GetHeight() const;

			void SetWidth(float _width);
			void SetHeight(float _height);

			void SetEdges();
			void NewDims(float _width, float _height);
	};

	class PLATFORM_API CONTAINER
	{
		private:
			ATOM* base_ptr{ nullptr };
			ATOM* end_ptr{ nullptr };

			int num_of_elements{ 0 };
			int next_push_back{ 0 };

		public:

			CONTAINER(int max_size);
			~CONTAINER();

			void push_back(ATOM& element);
			void push_front(ATOM& element);

			int size() const;

			ATOM& operator[] (int position);
			void operator()(ATOM& element, int position);
	};

	class PLATFORM_API FIELD :public ATOM
	{
		protected:
			fields type{};
			unsigned char flags{ 0 };

		public:
			dirs dir{};
			float speed{ 0 };

			FIELD(fields what_type);
			virtual ~FIELD() {};

			void Release();
			fields GetType() const;

			bool GetFlag(unsigned char which_flag) const;
			void SetFlag(unsigned char which_flag);
			void NullFlag(unsigned char which_flag);
			void Move(float gear);
	};

}