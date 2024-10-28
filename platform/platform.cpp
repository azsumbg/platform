#include "pch.h"
#include "platform.h"

//RAND ENGINE *************

gamedll::RANDENGINE::RANDENGINE()
{
	vSeeds.clear();
	for (int i = 0; i < 650; i++)vSeeds.push_back(rand_device());
	seeder = new std::seed_seq{ vSeeds.begin(), vSeeds.end() };
	twister = new std::mt19937{ *seeder };
}
gamedll::RANDENGINE::~RANDENGINE()
{
	if (seeder)delete seeder;
	if (twister)delete twister;
}
int gamedll::RANDENGINE::operator()(int min, int max)
{
	std::uniform_int_distribution distributor{ min, max };
	return distributor(*twister);
}

///////////////////////////

//ATOM ********************

gamedll::ATOM::ATOM(float _x, float _y, float _width, float _height)
{
	x = _x;
	y = _y;
	width = _width;
	height = _height;
	ex = x + width;
	ey = y + height;
}
float gamedll::ATOM::GetWidth() const
{
	return width;
}
float gamedll::ATOM::GetHeight() const
{
	return height;
}
void gamedll::ATOM::SetWidth(float _width)
{
	width = _width;
	ex = x + width;
}
void gamedll::ATOM::SetHeight(float _height)
{
	height = _height;
	ey = y + height;
}
void gamedll::ATOM::NewDims(float _width, float _height)
{
	width = _width;
	height = _height;
	ex = x + width;
	ey = y + height;
}
void gamedll::ATOM::SetEdges()
{
	ex = x + width;
	ey = y + height;
}

/////////////////////////////

//CONTAINER ***************

gamedll::CONTAINER::CONTAINER(int max_size) :num_of_elements{ max_size }, base_ptr{ new gamedll::ATOM[max_size] }
{
	end_ptr = base_ptr + (num_of_elements - 1);
}
gamedll::CONTAINER::~CONTAINER()
{
	if (base_ptr)delete[] base_ptr;
}
void gamedll::CONTAINER::push_back(ATOM& element)
{
	if (next_push_back + 1 < num_of_elements)
	{
		++next_push_back;
		*(base_ptr + next_push_back) = element;
	}
}
void gamedll::CONTAINER::push_front(ATOM& element)
{
	*base_ptr = element;
}
int gamedll::CONTAINER::size() const
{
	return num_of_elements;
}
gamedll::ATOM& gamedll::CONTAINER::operator[](int position)
{
	gamedll::ATOM ret;

	if (position < num_of_elements) ret = *(base_ptr + position);
	
	return ret;
}
void gamedll::CONTAINER::operator() (ATOM& element, int position)
{
	if (position < num_of_elements) *(base_ptr + position) = element;
}

/////////////////////////////

//FIELD *********************

gamedll::FIELD::FIELD(fields what_type) :ATOM(scr_width, 50.0f, 1000.0f, 750.0f)
{
	type = what_type;
	dir = dirs::left;
}
void gamedll::FIELD::Release()
{
	delete this;
}
fields gamedll::FIELD::GetType() const
{
	return type;
}
void gamedll::FIELD::SetFlag(unsigned char which_flag)
{
	flags |= which_flag;
}
bool gamedll::FIELD::GetFlag(unsigned char which_flag) const
{
	return flags & which_flag;
}
void gamedll::FIELD::NullFlag(unsigned char which_flag)
{
	flags &= ~which_flag;
}
void gamedll::FIELD::Move(float gear)
{
	float my_speed = speed + gear;
	if (dir == dirs::left)
	{
		x -= my_speed;
		SetEdges();
		if (ex <= 0)
		{
			SetFlag(LEFT_FLAG);
			NullFlag(RIGHT_FLAG);
		}
	}
	if (dir == dirs::right)
	{
		x += my_speed;
		SetEdges();
		if (x >= scr_width)
		{
			SetFlag(RIGHT_FLAG);
			NullFlag(LEFT_FLAG);
		}
	}
}

/////////////////////////////