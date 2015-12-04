// actor.h
#ifndef INCLUDED_ACTOR
#define INCLUDED_ACTOR

#include <engine/data/json_entity.h>
#include <engine/rendering/renderable_sprite.h>
#include <engine/scene/collision.h>
#include <engine/scene/icollider.h>
#include <engine/scene/world_view.h>

namespace StevensDev
{

namespace mgo
{

typedef unsigned int ActorID;
  // Defines a unique actor ID.
  // This is defined as a type since it may change in the future.

class Actor : public sgds::ICollider
{
  private:
    // CONSTANTS
    static const ActorID INVALID = ActorID( -1 );
      // Defines an invalid actor id.

    // GLOBALS
    static ActorID d_nextID;
      // Global actor id counter.

    // MEMBERS
    sgdr::RenderableSprite d_sprite;
      // The actor's sprite.

    sgds::RectangleBounds d_bounds;
      // The actor's collision bounds.

    sgdd::JsonEntity d_properties;
      // The actor instance's properties.

    std::string d_name;
      // The actor's name (used to identify type).

    ActorID d_id;
      // The actor's unique id.

    unsigned short d_flags;
      // The actor's collision bounds.

    // GLOBAL HELPER FUNCTIONS
    static ActorID nextID();
      // Gets the next actor ID.
      //
      // This is defined so that ActorID's type can be easily modified.

  public:
    // CONSTRUCTORS
    Actor();
      // Constructs an invalid actor (id = INVALID).

    Actor( const std::string& name,
           const sgdr::RenderableSprite& sprite,
           const sgds::RectangleBounds& bounds,
           unsigned short flags,
           const sgdd::JsonEntity& properties );
      // Constructs a new actor with the given information.
      //
      // The bounds are in world units.

    Actor( const Actor& actor );
      // Constructs a copy of the given actor.

    ~Actor();
      // Destructs the actor.

    // OPERATORS
    Actor& operator=( const Actor& actor );
      // Makes this a copy of the other actor.

    // ACCESSOR FUNCTIONS
    ActorID id() const;
      // Gets the actor's unique id.

    const std::string& name() const;
      // Gets the actor's name.

    virtual const sgdr::RenderableSprite& sprite() const;
      // Gets the actor's sprite.

    virtual const sgds::RectangleBounds& bounds() const;
      // Gets the actor's collision bounds (WU).

    virtual float getPositionX() const;
      // Gets the actor's x position (WU).

    virtual float getPositionY() const;
      // Gets the actor's y position (WU).

    virtual unsigned short flags() const;
      // Gets the actor's collision flags.

    virtual const sgdd::JsonEntity& properties() const;
      // Gets the actor's properties.

    // MUTATOR FUNCTIONS
    virtual void setFlags( unsigned short flags );
      // Sets the actor's collision flags.

    virtual void setPosition( float x, float y );
      // Sets the actor's position in the world (WU).

    virtual void move( float x, float y );
      // Moves the actor by the specified offset (WU).

    // MEMBER FUNCTIONS
    virtual bool canCollide( unsigned short flags ) const;
      // Checks if this actor can collide with an item with the given flags.

    virtual bool doesCollide( const sgds::RectangleBounds& candidate ) const;
      // Checks if this actor does collide with the given bounds.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const Actor& actor )
{
    sgdd::JsonPrinter p( stream );
    p.open().print( "id", actor.id() )
            .print( "name", actor.name() )
            .print( "flags", actor.flags() )
            .print( "bounds", actor.bounds() )
            .print( "properties", actor.properties() )
            .close();
    return stream;
}

// CONSTRUCTORS
inline
Actor::Actor() : d_sprite(), d_bounds(), d_properties(), d_name(),
                 d_id( ActorID( INVALID ) ),
                 d_flags( sgds::Collision::NO_COLLISION )
{
}

inline
Actor::Actor( const std::string& name, const sgdr::RenderableSprite& sprite,
              const sgds::RectangleBounds& bounds, unsigned short flags,
              const sgdd::JsonEntity& properties )
    : d_sprite( sprite ), d_bounds( bounds ), d_properties( properties ),
      d_name( name ), d_id( nextID() ), d_flags( flags )
{
}

inline
Actor::Actor( const Actor& actor )
    : d_sprite( actor.d_sprite ), d_bounds( actor.d_bounds ),
      d_properties( actor.d_properties ), d_name( actor.d_name ),
      d_id( actor.d_id ), d_flags( actor.d_flags )
{
}

inline
Actor::~Actor()
{
}

// OPERATORS
inline
Actor& Actor::operator=( const Actor& actor )
{
    d_sprite = actor.d_sprite;
    d_bounds = actor.d_bounds;
    d_properties = actor.d_properties;
    d_name = actor.d_name;
    d_id = actor.d_id;
    d_flags = actor.d_flags;

    return *this;
}

// ACCESSOR FUNCTIONS
inline
ActorID Actor::id() const
{
    return d_id;
}

inline
const std::string& Actor::name() const
{
    return d_name;
}

inline
const sgdr::RenderableSprite& Actor::sprite() const
{
    return d_sprite;
}

inline
const sgds::RectangleBounds& Actor::bounds() const
{
    return d_bounds;
}

inline
float Actor::getPositionX() const
{
    return d_bounds.x();
}

inline
float Actor::getPositionY() const
{
    return d_bounds.y();
}

inline
unsigned short Actor::flags() const
{
    return d_flags;
}

inline
const sgdd::JsonEntity& Actor::properties() const
{
    return d_properties;
}

// MUTATOR FUNCTIONS
inline
void Actor::setFlags( unsigned short flags )
{
    d_flags = flags;
}

inline
void Actor::setPosition( float x, float y )
{
    move( x - getPositionX(), y - getPositionY() );
}

inline
void Actor::move( float x, float y )
{
    sgds::WorldView& w = sgds::WorldView::inst();
    d_sprite.move( w.wuToDP( x ), w.wuToDP( y ) );
    d_bounds.setPosition( d_bounds.x() + x, d_bounds.y() + y );
}

// MEMBER FUNCTIONS
inline
bool Actor::canCollide( unsigned short flags ) const
{
    return sgds::Collision::canCollide( d_flags, flags );
}

inline
bool Actor::doesCollide( const sgds::RectangleBounds& candidate ) const
{
    return d_bounds.doesCollide( candidate );
}

// GLOBAL FUNCTIONS
inline
ActorID Actor::nextID()
{
    return d_nextID++;
}

} // End nspc mgo

} // End nspc StevensDev

#endif