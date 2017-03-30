#include "Stdafx.h"
#include "Particle.h"
/**
* Particle class defines a standard particle which controls its position, velocity, acceleration and colour
* @file: Particle.cpp
* @author: Ryan Thorn
* @date: 27/03/2017
* @copyright: Copyright Ryan Thorn (c) 2017. All rights reserved.
*/

Particle::Particle(glm::vec2 _position, glm::vec2 _velocity, glm::vec2 _acceleration, glm::vec3 _colour, float _velocityMax, float _radius)
{
	m_position = _position;
	m_velocity = _velocity;
	m_acceleration = _acceleration;

	m_colour = _colour;
	m_velocityMax = _velocityMax;
	m_radius = _radius;
}

Particle::~Particle()
{
}

// Updates the particle based on delta time
void Particle::Update(float _deltaTime)
{
	// Acceleration - Velocity calculation
	if (m_velocity.x < m_velocityMax)
	{
		m_velocity.x += m_acceleration.x * _deltaTime;
	}

	if (m_velocity.y < m_velocityMax)
	{
		m_velocity.y += m_acceleration.y * _deltaTime;
	}
	
	// Velocity - Position calculation
	if (m_position.x > 1280 || m_position.x < 0)
	{
		m_velocity.x = -m_velocity.x;
	}
	if (m_position.y > 768 || m_position.y < 0)
	{
		m_velocity.y = -m_velocity.y;
	}

	m_position += m_velocity * _deltaTime;
}

// Draws the particle to the screen using our renderer
void Particle::Draw(SDL_Renderer* _renderer)
{
	
	filledCircleRGBA(_renderer, m_position.x, m_position.y, m_radius, m_colour.r, m_colour.g, m_colour.b, 255);
	// Set the particles colour
	//SDL_SetRenderDrawColor(_renderer, m_colour.r, m_colour.g, m_colour.b, 255);
	// Draw the particle to screen
	//SDL_RenderDrawPoint(_renderer, m_position.x, m_position.y);
}
