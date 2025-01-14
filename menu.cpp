#include "menu.h"
MenuTree::MenuTree()
{
	nowMenu = new MenuNode("login");
	nowMenu->addChild("manager_main");
	nowMenu->addChild("user_main");
	nowMenu->addChild("user_register");
}
void MenuTree::deleteTree(MenuNode*& node)
{
	if (!node)
		return;
	for (MenuNode* cd : node->child)
	{
		deleteTree(cd);
	}
	delete node;
}
MenuTree::~MenuTree()
{
	MenuNode* temp = nowMenu;
	while (this->nowMenu->father != nullptr)
	{
		temp = temp->father;
	}
	deleteTree(temp);
}