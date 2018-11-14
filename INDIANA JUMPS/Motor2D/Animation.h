#ifndef __ANIMATION_H__
#define __ANIMATION_H__
#include "SDL/include/SDL_rect.h"
#include "j1App.h"

#define MAX_FRAMES 30

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
private:
	pugi::xml_document animation_file;

	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;
public:
	Animation()
	{}
	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}
	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}
	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}
		return frames[(int)current_frame];
	}
	bool Finished() const
	{
		return loops > 0;
	}
	void Reset()
	{
		loops = 0;
		current_frame = 0.0f;
	}
	void Animation::LoadAnimation(p2SString type) {


		pugi::xml_parse_result result = animation_file.load_file("config_animations.xml");
		if (result != NULL)
		{
			pugi::xml_node player = animation_file.child("sprites").child("player");
			speed = player.attribute("anim_speed").as_float();

			pugi::xml_node type_ = player.child(type.GetString());
			loop = type_.attribute("loop").as_bool();

			for (pugi::xml_node sprite = type_.child("sprite"); sprite; sprite = sprite.next_sibling("sprite"))
			{
				PushBack({ sprite.attribute("x").as_int(), sprite.attribute("y").as_int(), sprite.attribute("w").as_int(), sprite.attribute("h").as_int() });
			}
		}
	}
};
#endif