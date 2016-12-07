////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#include "../include/AnimatedSprite.h"

using namespace std;
using namespace sf;


AnimatedSprite::~AnimatedSprite()
{
    //dtor
}

AnimatedSprite::AnimatedSprite(Time frameTime, bool paused, bool looped)
{
    _animation=NULL;
    _frameTime=frameTime;
    _currentFrame=0;
    _isPaused=paused;
    _isLooped=looped;
    _texture=NULL;
}

void AnimatedSprite::setAnimation(const Animation& animation)
{
    _animation = &animation;
    _texture = _animation->getSpriteSheet();
    _currentFrame=0;
    setFrame(_currentFrame);
}

void AnimatedSprite::play(const Animation& animation)
{
    if(getAnimation() != &animation)
        setAnimation(animation);
    play();
}

void AnimatedSprite::stop()
{
    _isPaused=true;
    _currentFrame=0;
    setFrame(_currentFrame);
}

void AnimatedSprite::setColor(const Color& color)
{
    _vertices[0].color=color;
    _vertices[1].color=color;
    _vertices[2].color=color;
    _vertices[3].color=color;
}

FloatRect AnimatedSprite::getLocalBounds(float sizeRatioW, float sizeRatioH) const
{
    IntRect rect = _animation->getFrame(_currentFrame);

    float width = static_cast<float>(abs(rect.width));
    float height = static_cast<float>(abs(rect.height));

    return FloatRect(0.f, 0.f, width, height);
}

void AnimatedSprite::setFrame(size_t newFrame, bool resetTime)
{
    if(_animation)
    {
        IntRect rect = _animation->getFrame(newFrame);

        _vertices[0].position = Vector2f(0.f,0.f);
        _vertices[1].position = Vector2f(0.f,static_cast<float>(rect.height));
        _vertices[2].position = Vector2f(static_cast<float>(rect.width),static_cast<float>(rect.height));
        _vertices[3].position = Vector2f(static_cast<float>(rect.width),0.f);

        float left = static_cast<float>(rect.left) + 0.0001f;
        float right = left + static_cast<float>(rect.width);
        float top = static_cast<float>(rect.top);
        float bottom = top + static_cast<float>(rect.height);

        _vertices[0].texCoords = Vector2f(left, top);
        _vertices[1].texCoords = Vector2f(left, bottom);
        _vertices[2].texCoords = Vector2f(right, bottom);
        _vertices[3].texCoords = Vector2f(right, top);
    }

    if(resetTime)
        _currentTime = Time::Zero;
}

void AnimatedSprite::update(Time deltaTime)
{
    if(!_isPaused && _animation)
    {
        _currentTime += deltaTime;
        if(_currentTime >= _frameTime)
        {
            _currentTime = microseconds(_currentTime.asMicroseconds() % _frameTime.asMicroseconds());
            if(_currentFrame +1 < _animation->getSize())
                _currentFrame++;
            else
            {
                _currentFrame=0;

                if(!_isLooped)
                {
                    _isPaused=true;
                }
            }
            setFrame(_currentFrame, false);
        }
    }
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(_animation && _texture)
    {
        states.transform *= getTransform();
        states.texture = _texture;
        target.draw(_vertices, 4, Quads, states);
    }
}
