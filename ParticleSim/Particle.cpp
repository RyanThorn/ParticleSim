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
	m_nextPosition = _position;
	m_velocity = _velocity;
	m_acceleration = _acceleration;

	m_colour = _colour;
	m_velocityMax = _velocityMax;
	m_radius = _radius;
	m_app = Application::Instance();
	m_profiler = m_app->GetProfiler();
	m_screenWidth = (int)m_app->GetWindowSizes().x;
	m_screenHeight = (int)m_app->GetWindowSizes().y;
}

Particle::~Particle()
{
}

// Updates the particle based on delta time
void Particle::Update(float _deltaTime, SpatialHashTable &_sht)
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

	// Apply the velocity to our predicted next position, to check ahead of
	// time to see if a collision will occur
	m_nextPosition += m_velocity * _deltaTime;

	// check to see if this particle is colliding with another particle, if it is we need to invert its
	// velocity
	// Get its neighbours
	std::vector<Particle*>::iterator m_iterator;
	std::vector<Particle*> m_neighbours;
	// Get the neighbours from our spatialhashtable
	m_neighbours = _sht.GetLocalObjects(this);

	for (m_iterator = m_neighbours.begin(); m_iterator != m_neighbours.end(); ++m_iterator)
	{
		// Check it is not itself
		if ((*m_iterator) != this)
		{
			// Update our collision counter
			m_profiler->AddCollision();
			// Check the collision with this particle and the particle in the bucket
			CheckCollision(*m_iterator);
		}
	}
	
	// Check to see if a particle is hitting the wall. if it is provide the correct response
	if (m_position.x > m_screenWidth)
	{
		m_velocity.x = -m_velocity.x;
		m_position.x = m_screenWidth - 1;
	}
	else if (m_position.x < 0)
	{
		m_velocity.x = -m_velocity.x;
		m_position.x = 1;
	}
	
	if (m_position.y > m_screenHeight)
	{
		m_velocity.y = -m_velocity.y;
		m_position.y = m_screenHeight - 1;
	}
	else if (m_position.y < 0)
	{
		m_velocity.y = -m_velocity.y;
		m_position.y = 1;
	}

	// Velocity - Position calculation
	m_position += m_velocity * _deltaTime;
}

// Draws the particle to the screen using our renderer
void Particle::Draw(SDL_Renderer* _renderer)
{
	// Set the particles colour
	SDL_SetRenderDrawColor(_renderer, m_colour.r, m_colour.g, m_colour.b, 255);
	// Draw the particle to screen
	SDL_RenderDrawPoint(_renderer, m_position.x, m_position.y);
}

/**
* Check if this particle is colliding with another particle given as a parameter
* @param Particle* _particle The particle to check against
* @returns bool Returns true if it is colliding, false if not
*/
bool Particle::CheckCollision(Particle* _particle)
{
	// Checks THIS particle against ONE other particle. Should be used in 
	// conjunction with our spatial hash table to check all surrounding particles

	// Get combined radii
	float m_combinedRadii = this->Radius() + _particle->Radius();
	// Calculate the difference between both circle centers
	glm::vec2 m_diff = glm::vec2(m_position.x - _particle->Position().x, m_position.y - _particle->Position().y);
	// Calculate the distance using pythagoras' beautiful theorem
	float m_distance = sqrtf(m_diff.x * m_diff.x + m_diff.y * m_diff.y);

	// Check if the overall distance is less than both the particles radii combined
	if (m_distance < m_combinedRadii)
	{
		// Collision has been detected lets handle it
		HandleCollision(m_diff, m_distance, m_combinedRadii, _particle);
		// Also return true
		return true;
	}
	// No collision, return false
	return false;
}

/**
* Handles the collision response for the two particles. Calculates linear momentum as a response
* @param _diff glm::vec2 The difference in x and y components of the two particles
* @param _distance float The distance between the two particles
* @param _combinedRadii float The combined radii of both particles
* @param _otherParticle Particle* The other particle included in the collision response
*/
void Particle::HandleCollision(glm::vec2 _diff, float _distance, float _combinedRadii, Particle* _otherParticle)
{
	// normalise the difference vector
	glm::vec2 m_norm = glm::vec2(_diff.x / _distance, _diff.y / _distance);
	
	// Calculate how much the two particles are colliding by
	float m_amount = -(_distance - _combinedRadii);

	// Add the normal vector multiplied by the amount the particles are colliding by
	m_position += m_norm * m_amount;

	// And negate it from the other particle
	_otherParticle->Position(_otherParticle->Position() - m_norm * m_amount);

	// Do the velocity inversion seperately for each axis
	// Invert the velocities for X if the difference between x positions is greater than the combined radii /2
	if (abs(_diff.x) >= _combinedRadii / 2)
	{
		m_velocity.x *= -1.0f;
		_otherParticle->VelocityX(_otherParticle->Velocity().x * -1.0f);
	}

	// Invert the velocities for Y if the difference between x positions is greater than the combined radii /2
	if (abs(_diff.y) >= _combinedRadii / 2)
	{
		m_velocity.y *= -1.0f;
		_otherParticle->VelocityY(_otherParticle->Velocity().y * -1.0f);
	}
}