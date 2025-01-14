#pragma once
#include <string>
#include <vector>
class MenuNode
{
public:
	std::string menuName;
	MenuNode* father;
	std::vector<MenuNode*> child;

	void addChild( std::string childName)
	{
		MenuNode* cd = new MenuNode(childName);
		cd->father = this;
		child.push_back(cd);
	}

	MenuNode(const std::string name)
	{
		menuName = name;
		father = nullptr;
	}
};

class MenuTree {
public:
	MenuNode* nowMenu;
	MenuTree();
	~MenuTree();
	void deleteTree(MenuNode*& node);
};

