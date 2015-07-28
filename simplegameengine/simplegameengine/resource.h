#pragma once

#include <vector>
#include "animatingicon.h"
#include "map2d.h"

class Resource {
public:
	struct Entry {
		const char * name;
		enum class Type { NONE, animation, map2d };
		Type type;
		void * data;
		Entry(const char * name, Type type, void * data) : name(name), type(type), data(data) {}
	};

	std::vector<Entry> manifest;

	void * Get(const char * name, Entry::Type type) {
		for (int i = 0; i < manifest.size(); ++i){
			if (strcmp(manifest[i].name, name) == 0) {
				return manifest[i].data;
			}
		}
		void * data = nullptr;
		switch (type){
		case Entry::Type::animation:
			data = new AnimatingIcon(name);
			break;
		case Entry::Type::map2d:
			{
				Map2D * m = new Map2D;
				m->LoadFile(name);
				data = m;
			}
			break;
		}
		if (data != nullptr){
			manifest.push_back(Entry(name, type, data));
		}
		return data;
	}
	AnimatingIcon * GetAnimation(const char * name) { return (AnimatingIcon*)Get(name, Entry::Type::animation); }
	Map2D * GetMap2D(const char * name) { return (Map2D*)Get(name, Entry::Type::map2d); }
};