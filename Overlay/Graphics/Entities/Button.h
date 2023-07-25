#pragma once
#include "Entity.h"
class Button : public Entity
{
protected:


    std::function< void()> Action;
public:
    Button(float x, float y, std::wstring text, std::function<void()> click);
    void Update();
    void Draw();
};
