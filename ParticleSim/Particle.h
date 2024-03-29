#ifndef _PARTICLE_H_
#define _PARTICLE_H_
/**
* Particle class defines a standard particle which controls its position, velocity, acceleration and colour
* @file: Particle.h
* @author: Ryan Thorn
* @date: 27/03/2017
* @copyright: Copyright Ryan Thorn (c) 2017. All rights reserved.
*/
class Application;
class SpatialHashTable;
class Particle
{
private:
	// Particle's current position
	glm::vec2 m_position;
	// Particle's predicted next position
	glm::vec2 m_nextPosition;
	// Particle's current velocity (movement force)
	glm::vec2 m_velocity;
	// Particle's current acceleration (change in velocity)
	glm::vec2 m_acceleration;
	// The particles colour
	glm::vec3 m_colour;
	// Maximum velocity reached via acceleration
	float m_velocityMax;
	// Radius of the particle
	float m_radius;

	// Our application instance
	Application* m_app;
	FPSProfiler* m_profiler;
	int m_screenWidth, m_screenHeight;

	/**
	 * Handles the collision response for the two particles. Calculates linear momentum as a response
	 * @param _diff glm::vec2 The difference in x and y components of the two particles
	 * @param _distance float The distance between the two particles
	 * @param _combinedRadii float The combined radii of both particles
	 * @param _otherParticle Particle* The other particle included in the collision response
	 */
	void HandleCollision(glm::vec2 _diff, float _distance, float _combinedRadii, Particle* _otherParticle);
public:
	Particle(glm::vec2 _position = glm::vec2(0, 0), glm::vec2 _velocity = glm::vec2(0, 0), glm::vec2 _acceleration = glm::vec2(0,0), glm::vec3 _colour = glm::vec3(255,0,0), float _velocityMax = 50.0f, float _radius = 2.0f);
	~Particle();

	// Updates a particle based on delta time
	void Update(float _deltaTime, SpatialHashTable &_sht);

	// Draws a particle to the screen
	void Draw(SDL_Renderer* _renderer);

	/**
	 * Check if this particle is colliding with another particle given as a parameter
	 * @param Particle* _particle The particle to check against
	 * @returns bool Returns true if it is colliding, false if not
	 */
	bool CheckCollision(Particle* _particle);

	// Position Getter and Setter
	void Position(glm::vec2 _position) { m_position = _position; }
	glm::vec2 Position() { return m_position; }
	glm::vec2 NextPosition() { return m_nextPosition; }

	// Velocity Getter and Setter
	void Velocity(glm::vec2 _velocity) { m_velocity = _velocity; }
	void VelocityX(float _vel) { m_velocity.x = _vel; }
	void VelocityY(float _vel) { m_velocity.y = _vel; }
	glm::vec2 Velocity() { return m_velocity; }

	// Acceleration Getter and Setter
	void Acceleration(glm::vec2 _acceleration) { m_acceleration = _acceleration; }
	glm::vec2 Acceleration() { return m_acceleration; }

	// Colour Getter and Setter
	void Colour(glm::vec3 _colour) { m_colour = _colour; }
	glm::vec3 Colour() { return m_colour; }

	void Radius(float _radius) { m_radius = _radius; }
	float Radius() { return m_radius; }

};
#endif //!_PARTICLE_H_

