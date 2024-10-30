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
	if (type == fields::platform)NewDims(100.0f, 50.0f);

	switch (type)
	{
	case fields::field1:
		max_frames = 10;
		frame_delay = 5;
		break;

	case fields::field2:
		max_frames = 7;
		frame_delay = 8;
		break;

	case fields::field3:
		max_frames = 34;
		frame_delay = 2;
		break;
	}

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
int gamedll::FIELD::GetFrame()
{
	frame_delay--;
	if (frame_delay <= 0)
	{
		switch (type)
		{
		case fields::field1:
			frame_delay = 5;
			break;

		case fields::field2:
			frame_delay = 8;
			break;

		case fields::field3:
			frame_delay = 2;
			break;
		}
		++frame;
		if (frame > max_frames)frame = 0;
	}
	return frame;
}

/////////////////////////////

//CREATURE BASE CLASS *************

gamedll::CREATURE::CREATURE(creatures what, float start_x, float start_y) :ATOM(start_x, start_y)
{
	type = what;
	dir = dirs::left;

	switch (type)
	{
	case creatures::evil1:
		NewDims(45.0f, 55.0f);
		speed = 0.5f;
		lifes = 50;
		max_frames = 23;
		frame_delay = 3;
		strenght = 10;
		attack_delay = 20;
		break;

	case creatures::evil2:
		NewDims(22.0f, 53.0f);
		speed = 0.8f;
		lifes = 30;
		max_frames = 23;
		frame_delay = 3;
		strenght = 5;
		attack_delay = 15;
		break;

	case creatures::evil3:
		NewDims(35.0f, 54.0f);
		speed = 0.6f;
		lifes = 40;
		max_frames = 23;
		frame_delay = 3;
		strenght = 8;
		attack_delay = 25;
		break;

	case creatures::evil4:
		NewDims(40.0f, 41.0f);
		speed = 0.8f;
		lifes = 45;
		max_frames = 23;
		frame_delay = 3;
		strenght = 8;
		attack_delay = 10;
		break;

	case creatures::evil5:
		NewDims(36.0f, 66.0f);
		speed = 0.6f;
		lifes = 35;
		max_frames = 23;
		frame_delay = 3;
		strenght = 10;
		attack_delay = 25;
		break;

	case creatures::evil6:
		NewDims(43.0f, 55.0f);
		speed = 0.7f;
		max_frames = 23;
		frame_delay = 3;
		strenght = 12;
		attack_delay = 25;
		break;

	case creatures::hero:
		NewDims(47.0f, 47.0f);
		dir = dirs::right;
		speed = 10.0f;
		max_frames = 23;
		frame_delay = 3;
		strenght = 20;
		attack_delay = 10;
		break;
	}
}
void gamedll::CREATURE::SetPathInfo(float where_x, float where_y)
{
	start_x = x; 
	start_y = y;
	end_x = where_x;
	end_y = where_y;

	vert_path = false;
	hor_path = false;

	if (end_x - start_x == 0)
	{
		vert_path = true;
		return;
	}
	if (end_y - start_y == 0)
	{
		hor_path = true;
		return;
	}

	slope = (end_y - start_y) / (end_x - start_x);
	intercept = start_y - start_x * slope;
}
creatures gamedll::CREATURE::GetType() const
{
	return type;
}
int gamedll::CREATURE::Attack()
{
	attack_cooldown++;
	if (attack_cooldown > attack_delay)
	{
		attack_delay = 0;
		return strenght;
	}
	return 0;
}
bool gamedll::CREATURE::Falling(CONTAINER& platforms)
{
	if (platforms.size() < 1)return true;
	for (int i = 0; i < platforms.size(); i++)
	{
		if (!(x > platforms[i].ex || ex < platforms[i].x || y > platforms[i].ey || ey < platforms[i].y))
		{
			if ((dir == dirs::up || dir == dirs::down) && y <= platforms[i].y)return false;
		}
	}
	return true;
}

///////////////////////////////////

//EVILS **********************

void gamedll::EVILS::Release()
{
	delete this;
}
bool gamedll::EVILS::GetFlag(unsigned char which_flag) const 
{
	return flags & which_flag;
}
void gamedll::EVILS::SetFlag(unsigned char which_flag)
{
	flags |= which_flag;
}
void gamedll::EVILS::NullFlag(unsigned char which_flag)
{
	flags &= ~which_flag;
}
int gamedll::EVILS::GetFrame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = 3;
		++frame;
		if (frame > max_frames)frame = 0;
	}
	return frame;
}
unsigned char gamedll::EVILS::Move(float gear, bool change_path, float path_ex, float path_ey)
{
	flags = 0b00000000;
	if (change_path)SetPathInfo(path_ex, path_ey);
	float current_speed = speed + gear / 10;

	if (vert_path)
	{
		if (dir == dirs::up)
		{
			y -= current_speed;
			SetEdges();
			if (ey <= sky)SetFlag(UP_FLAG);
		}
		else if (dir == dirs::down)
		{
			y += current_speed;
			SetEdges();
			if (y >= ground)SetFlag(DOWN_FLAG);
		}
	}
	else if (hor_path)
	{
		if (dir == dirs::left)
		{
			x -= current_speed;
			SetEdges();
			if (ex <= -(2 * scr_width))SetFlag(LEFT_FLAG);
		}
		else if (dir == dirs::right)
		{
			x += current_speed;
			SetEdges();
			if (x >= 2 * scr_width)SetFlag(RIGHT_FLAG);
		}
	}
	else
	{
		if (dir == dirs::left)
		{
			x -= current_speed;
			y = x * slope + intercept;
			SetEdges();
			if (ex <= -(2 * scr_width))SetFlag(LEFT_FLAG);
		}
		else if (dir == dirs::right)
		{
			x += current_speed;
			y = x * slope + intercept;
			SetEdges();
			if (ex <= 2 * scr_width)SetFlag(RIGHT_FLAG);
		}
	}

	return flags;
}

///////////////////////////////


