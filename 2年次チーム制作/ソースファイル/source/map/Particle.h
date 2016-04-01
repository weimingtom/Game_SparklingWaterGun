#pragma once

class Particle
{
public:
	FLOAT mass;
	Vector3 position;
	Vector3 velocity;

private:
	Vector3 acceleration;
	Vector3 resultant;

public:
	Particle() : mass( FLT_MAX ), position( 0, 0, 0 ), velocity( 0, 0, 0 ),
		acceleration( 0, 0, 0 ), resultant( 0, 0, 0 ) {}

	virtual ~Particle() {}

	void Integrate( FLOAT dt )
	{
		assert( mass > 0 );

		acceleration = ( resultant / mass );
		velocity += acceleration * dt;
		position += velocity * dt;

		resultant = Vector3( 0, 0, 0 );
	}
	void AddForce( const Vector3 &force )
	{
		resultant += force;
	}

	const Vector3 &GetAcceleration() const
	{
		return acceleration;
	}
};
