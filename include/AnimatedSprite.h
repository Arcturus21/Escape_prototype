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

#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Updatable.h"
#include "Animation.h"

class AnimatedSprite : public sf::Drawable, public sf::Transformable, public Updatable
{
    public:
        virtual ~AnimatedSprite();

        explicit AnimatedSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true);

        void update(sf::Time deltaTime);
        void setAnimation(const Animation& animation);

        void setFrameTime(sf::Time time){_frameTime=time;};
        void play(){_isPaused=false;};

        void play(const Animation& animation);

        void pause(){_isPaused=true;};

        void stop();

        void setLooped(bool looped){_isLooped=looped;};

        void setColor(const sf::Color& color);

        const Animation* getAnimation() const{return _animation;};

        sf::FloatRect getLocalBounds(float sizeRatioW=1, float sizeRatioH=1) const;

        sf::FloatRect getGlobalBounds(float sizeRatioW=1, float sizeRatioH=1) const{return getTransform().transformRect(getLocalBounds(sizeRatioW, sizeRatioH));};
        bool isLooped() const{return _isLooped;};
        bool isPlaying() const{return !_isPaused;};
        sf::Time getFrameTime() const{return _frameTime;};

        void setFrame(std::size_t newFrame, bool resetTime=true);

    protected:
        const Animation* _animation;
        sf::Time _frameTime;
        sf::Time _currentTime;
        std::size_t _currentFrame;
        bool _isPaused;
        bool _isLooped;
        const sf::Texture* _texture;
        sf::Vertex _vertices[4];

    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // ANIMATEDSPRITE_H
