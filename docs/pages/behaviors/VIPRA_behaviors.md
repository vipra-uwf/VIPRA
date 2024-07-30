**File Path:** [Home](../../docs.md) / [Behaviors](VIPRA_behaviors.md)
# VIPRA Behaviors

VIPRA Behaviors is a DSL for creating human behaviors for use in pedestrian dynamics simulations.
Its goal is to provide an easy way for researchers without programming experience to test how human behaviors may affect pedestrian dynamics.

`Behaviors` are a way to describe `Actions` that pedestrians take under a certain `Condition` without the use of a traditional programming language.

`Behaviors` are comprised of four major parts:
1. [Types](#types)
2. [Selectors](#selectors)
3. [Events](#events)
4. [Actions](#actions)

See also [Behavior Examples](behavior_examples.md)

---

# Behavior Layout

The general layout of a `Behavior` is as follows:
```
Types Declaration.

States Declaration.

Location Declarations.

Selector Declarations.

Event Declartions.

Action Declarations.
```

Each Declaration has a subset of components.

```
*Declaration Type*:
  *Component Name* : *Component Value*
  *...*
.
```

Each section goes over what components are available for each.


```
Selector:
  Type: injured_person
  Select: 10%
.
```

// TODO(tyler): Consider removing this, may be redundant with the actual selector section.   
`Type` and `Select` are both components of the [`Selector`](#selectors) declaration


---

# Sy. General Syntax Rules

This section has some simple `Syntax` rules that must be followed for a `Behavior` to be considered correct.

The other sections will have the specific `Syntax` rules for their respective parts, in a `Section - General Syntax Rules` section

1. All Declarations must end in a `.`
```
correct:
  Event:
    Name: Example
    *...*
  .

incorrect:
  Event:
    Name: Example
    *...*
```

2. Each `Behavior` (§B) must start with a `Types Declaration` (§T.1)
3. Each `Behavior` (§B) must only have 1 `Types Declaration` (§T.1)
4. Each `Behavior` (§B) must have at least 1 `Selector` (§S)
5. `Behaviors` are NOT case-sensitive so `Consider`, `consider`, and `ConSiDEr` are all valid.
6. Comments can be added following '//' or between '/*' and '\*/'
```
// This is a comment and does not affect the behavior

/*
This is a multi-line comment
and does not affect the behavior
*/
```
7. Whitespace does not affect `Behaviors`, the following are valid and equivalent.
```
Types: typeA typeB.

// and

Types:
  typeA
  typeB
.
```
8. Declaration Components can be written in any order, the following are equivalent
```
Event:
  Name: Example
  Start: *start condition*
.

Event:
  Start: *start condition*
  Name: Example
.
```

---

# T. Types

`Types` are how pedestrians are organized in `Behaviors` (§B)

Each pedestrian is assigned a user defined `Type`, and based on their `Type` they will follow different `Actions` (§A).

---

## T.1. Types Declaration

A `Types Declaration` is what says which types are being used in the `Behavior`.

`Behaviors` can only have 1 `Types Declaration` with up to 64 types.

`Types Declarations` are slightly different from other declarations, in that they do not have components; instead each type is simply listed out.

Single `Type`:
```
Types:
  injured_person
.
```

Multiple `Types`:
```
Types:
 injured_person
 helper
.
```

---

### T.1.1. Types Declaration - General Syntax Rules

1. `Types Declarations` can be any length up to 64 `Types`
```
Types:
  typeA
  typeB
  typeC
  typeD
.
```
2. `Types Declarations` must be the first declaration in a `Behavior`


---

## T.2. Composite Types

A Pedestrians `Type` can be composed of several other types.

A Pedestrian with a `Composite Type` will have the attributes of each individual `Type`.

How to assign `Composite Types` is explained in the `Selectors` (§S) section.


---

## T.3. Groups

For most users, `Groups` and `Types` refer to the same thing.

The main difference between a `Type` and a `Group` is that there is a base `Group` for each `Behavior`, being 'Pedestrian' (or 'Pedestrians').

Each pedestrian with a `Type` is considered in that `Types` `Group`.

Pedestrians with `Composite Types` are in a `Group` for each `Type`.


---

## T.4. Types - General Syntax Rules

1. `Type` names can only contain Letters, Underscores, and Hyphens `(a-z), (A-Z), '_', '-'`


---


# L. Locations

`Locations` are rectangular regions that can be used in `Selectors`, `Conditions`, and `Actions`.

After being declared, `Location` names are always preceded by a `@`

```
Location:
  Name: *name*
  Dimensions:
    Center: *center*         // Coord Value (§V.2)
    Lengths: *side lengths*  // Coord Value (§V.2)
    Rotation: *rotation*     // Numeric Value (§V.1)
.
```
- \*Name* - The name used to reference this `Location`
- \*Dimensions* - The dimensions for the `Location`
  - \*center* - X,Y,Z coordiantes for the center of the rectangle
  - \*side lengths* - X,Y,Z lengths of the rectangle
  - \*rotation* - degrees of clockwise rotation


Example:
```
Location:
  Name: Aisle
  Dimensions:
    Center: {15, 1.7}  // Center of rectangle is at (15, 1.7)
    Lenghts: {30, 0.3}  // Height of 30m, Width of 30cm
    Rotation: 0           // Rotated 0 degrees (aligned with X axis)
.
```
---

## L.1. Locations - General Syntax Rules

1. `Location` names are always preceded by a `@` after the declaration

---

---

# Selectors

`Selectors` are how pedestrians are selected for a certain `Type` (§T).

Selecting pedestrians is done through a `Selector` declaration. The basic syntax is as follows:
```
Selector:
  Type: *types*
  From: *type*                  // Optional, defaults to base pedestrians group
  Select: *selection criteria*
.
```

- \*Select* - The exact `Select` criteria to use, available `Select` criteria are in (§S.5.)
- \*From* - The `Group` (§T.3) to select pedestrians from
- \*Type* - The `Type` (§T.) to assign to selected pedestrians

Selectors are applied with precedence equal to the order they appear in the `Behavior` file.

Example:
```
Selector:   // Selects exactly 10 pedestrians to be of typeA
  Type: typeA
  Select: 10
.

Selector:   // Selects 50% of pedestrians to be of typeB
  Type: typeB
  Select 50%
.
```

---

## S.2. Selecting From Groups

`Selectors` can select from specific `Groups` (§T.3)

This has the effect of selecting the pedestrians for a `Composite Type` (§T.2) but allows for more dynamic proportions.
 

```
Types:
  typeA
  typeB
  typeC
.

Select:        // Selects 50% of pedestrians for typeA
  Type: typeA
  Select: 50%
.

Select:        // Selects 15% of typeA pedestrians for typeA & typeB
  Type: typeB
  From: typeA
  Select: 15%
.

Select:       // Selects 5% of typeA pedestrians for typeA & typeC
  Type: typeC
  From: typeA
  Select: 5%
.
```

With 100 pedestrians:
- 39 pedestrians are typeA.
- 8 pedestrians are typeA and typeB.
- 3 pedestrians are typeA and typeC.

---

## S.3. Selector Exclusivity

When a pedestrian is selected from a `Group`, it is marked as used and can not be selected by another `Selector`.

```
Select:
  Type: typeB
  Group: typeA
  Select: 15%
.
```
Means:
Those 15% of 'typeA' pedestrians can not be selected from again.

However, they can be selected from 'typeB'.

```
Select:
  Type: typeC
  Group: typeB
  Select: 15%
.
```

This may result in some pedestrians being of types: typeA, typeB, and typeC.

---

## S.4. Required Selectors

`Selectors` can be marked as `Required` with a 'Required' `Component`. 

This means, if the `Selector` is unable to be filled an error will be thrown, and the simulation will stop.

```
Types:
  typeA
  typeB
.

Selector:
  Type: typeA
  Select: Everyone
.

Selector:
  Required
  Type: typeB
  Select: 50%
.
```
Output:
```
Behavior: Example, Required Selector Starved For Type: 2 From Group: 0"
```

---

## S.5. Available Select Criteria

1. Everyone
2. Percent
3. Exactly N

---

### S.5.1. Everyone

```
Select: Everyone
```

This select criteria will select every pedestrian to have the selected `Type` (§T)


---

### S.5.2 Percent


Selects a percentage of a `Group` for the provided `Type`

```
*X*%
```
\*X* - Number `Value` (§V) (1 - 100)

 
Example:
```
Select: 15%
```


---

### S.5.3 Exactly N 

Selects an exact number of pedestrians from a `Group`.

```
Exaclty *X*
```
\*X* - Number `Value` (§V)

 
Example:
```
Select: 10
```


---

### S.5.4 Location

Selects all pedestrians inside a given location.

```
Select: In *Location*
```
\*Location\* - Name of `Location` (§L)

Example:
```
Select: In @Elevator
```

---

## S.6 Selectors - General Syntax Rules


---


# A. Actions

`Actions` are what affect a Pedestrian's position, velocity, state, etc.

`Actions` work by applying their `Atoms` each time step their `Condition` is satisfied.

---

## A.1. Unconditional Actions

An `Unconditional Action` will ALWAYS take effect, applying its `Atoms` each timestep.


```
Action (*type*):
  Response: *atoms*
.
```
- Type being the pedestrian type that follows this `Action`
- Atoms being the steps taken in an action, more in (§A.3.)


Example:
```
Action (injured_person):         // Action Declaration for "injured_person"s
  Response: scale velocity 0.75  // Scale Atom (§A.5.2.)
.
```
With this Behavior, an injured person will always walk at 75% speed.


---

## A.2. Conditional Actions

A `Conditional Action` will only take effect if its `Condition` is satisfied.

`Conditional Actions` are written as follows:
```
Action (*type*):
  Condition: *condition*
  Response: *atoms*
.
```

- \*Type* being the pedestrian type that follows this `Action`
- \*Atoms* being the steps taken in an action, more in (§A.4.)
- see (§C.) for an explanation of Conditions

---

Example:
```
Action (listener): 
  Condition: !announcement occurring. // Condition (§C.)
  Response: set velocity {0}          // Set Atom (§A.5.1.)
.
```
Pedestrians of listener will stop moving while the announcement event is occurring.


---

## A.3. Action Durations

Normally `Actions` only apply to the time step their `Condition` is true in.

If a longer response is needed, a `Duration` can be added to the `Action`.

```
Action (*type*): 
  Condition: *condition*
  Response: *atoms*
  Duration: *value*
.
```
- \*Type* being the `Type` (§T.) the action applies to
- \*Atoms* being the `Atoms` (§A.4.) for the `Action`
- \*Condition* being the `Condition` (§C.) for the action to start
- \*value* being a numerical `Value` (§V.), for how long the action should continue

---

Example, Someone Tripping:
```
Action (tripper):
  Condition: 10 seconds after !Start // Condition (§C.)
  Response: set velocity {0}         // Set Atom (§A.5.1.)
  Duration: 15 seconds               // Duration
.
```

Pedestrians of type `tripper` will trip after 10 seconds from the simulation starting, and remain still for 15 seconds.

This is odd, as all `trippers` will trip at exactly the same time:
Look at `Values` (§V.) for how to add randomness to `Durations` and `Conditions`


---

## A.4. Atoms

`Atoms` are the part of an `Action` that actually affects a pedestrian.

The do this by changing pedestrian `Attributes` (§Att.).

§A.5. has the list of available `Atoms`.

---

## A.4.1 Atom Lists

An `Atom List` is the collection of `Atoms` that make up an `Action`.

`Atom Lists` are written as follows:
`*Atom* , *Atom*`

**Important Note:** `Atom` effects are applied in the order they are listed

---

Example:

```
set velocity {0}, set state #listening
```
This `Atom List` will result in the pedestrian stopping and their state being listening

(Pedestrian state is useful in `Conditions` (§C.))


---

## A.5. Available Atoms

---

### A.5.1. Set

The `Set Atom` sets a pedestrian's attribute to a specific value.

```
set *attribute* to *attribute value*
```

- Attributes are described in (§Att.).
- Attribute Values are described in (§Att.1).


---

### A.5.2. Scale

The `Scale Atom` multiplies a pedestrian attribute by a given value

```
scale *attribute* *attribute_value*
```

Currently, only pedestrian `Velocity` can be scaled.

- Attributes are described in (§Att.).
- Attribute Values are described in (§Att.1 ).


---

## A.6. Action Targets

Sometimes Pedestrians will want to affect other pedestrians around them.

This is done through the use of `Targets`.

```
Action (*type*):
  Condition: *condition*  // Condition (§C.)
  Response: *atoms*       // Atoms (§A.4.)
  Target: *target*        // Action Target
```

---

Example:

```
Action (aggressive):
  Response: set target velocity {0}
  Target: nearest pedestrian
```
This results in the pedestrian stopping the nearest pedestrian around them


---

## A.6.1 Available Targets

---

## A.6.1.1 Nearest of Type

Selects the nearest of a given type to be the target

```
nearest *type list*   // selects the nearest pedestrian with any of the listed types

// or

nearest pedestrian    // selects any pedestrian
```
  
---

## A.6.2 Target Modifiers

`Target Modifiers` change how pedestrians are selected to be the Target of an action.

`Target Modifiers` are simply appended to the end of the `Action Target`.

Example:
```
Target: nearest pedestrain in front within 1.0
```
This selects the nearest pedestrian in the direction of motion that is within 1 meter.

---

## A.6.2.1 Distance Target Modifier

Targets can be filtered by their distance to the pedestrian.

```
within *numeric value*
```

---

## A.6.2.2 Direction Target Modifier

Targets can be filtered by their direction from the pedestrian.

Currently, there are only two directions.
```
in front   // used to select pedestrians in the direction the pedestrian is headed

behind     // used to select pedestrians in the opposite direction the pedestrian is headed
```

---

## A.6.2.3. Location Target Modifier

Targets can be filtered by whether they are inside a `Location` or not.

```
in *location name*
```

---

# C. Conditions

A `Condition` is what decides if a `Conditional Action` or `Event` occurs. 

`Conditions` are comprised of one or more `Sub Conditions` and the boolean operations between them (and/or).

`Conditions` do not stand on their own, and only appear as part of an `Action` or `Event` declaration.

---

Example:

```
Action (typeA):
  Condition: !example occurred and 5 seconds after !Start
  Response: set velocity {0}
  Duration: 10 seconds
.
```
This results in typeA pedestrians stopping for 10 seconds after the example event has occurred but not before 5 seconds have passed in the simulation.


---

## C.1 Sub Conditions

A `Sub Condition` defines a single condition of the state of the simulation in which it returns true.

Available `Sub Conditions` are in section (§C.2.)


---

## C.2 Available Sub Conditions

---

### C.2.1. Elapsed Time

This `Condition` will be true when a provided amount of time has passed from the start of an `Event` (§E.)

**Note:** This is true only for one time step when the time has elapsed, until the `Event` starts again.

```
*numeric value* seconds after *event*.
```

---

Example:
```
Action (typeA):
  Condition: 10 seconds after !Start // Elapsed Time Condition
  Response: set velocity {0}         // Atom (§A.4)
  Duration: 5 seconds                // Duration (§A.3)
.
```
**Note:** without the `Duration` the pedestrian would stop moving for only one time step.


---

### C.2.2. Spatial

This `Condition` will be true when the condition Target is within a certain distance

```
target within *numeric value*.
```

---

Example:
```
Action (typeA):
  Condition: 10 seconds after !Start // Elapsed Time Condition
  Response: set velocity {0}         // Atom (§A.4)
  Duration: 5 seconds                // Duration (§A.3)
.
```
**Note:** without the `Duration` the pedestrian would stop moving for only one time step.


---

### C.2.3. Event Status

This `Condition` checks for the status of an `Event` (§E.)

```
*event* is *status*
```

### Available Statuses:
```
Starting  // True for the timestep the event starts
Ending    // True for the timestep the event ends
Occurring // True for every timestep between event start and end
Occurred  // True if the event has occurred at all during the simulation
```

---

Example:
```
Action (listener):
  Condition: !announcement is occurring
  Response: set velocity to {0}
.
```

---

## C.2.4. Location Enter/Leave

This `Condition` checks if a pedestrian has entered or left a `Location` (§L.)

```
Enter *location name*

// or

Exit *location name*
```


Example:
```
Action (Luggage_Grabber):
  Condition: Enter @Aisle
  Response: set velocity to {0}
  Duration: 10-30 seconds
.
```

---

## C.2.5. Inside Location

This `Condition` checks if a pedestrian is inside a `Location` (§L.)

```
In *location name*
```

Example:
```
Action (Patient):
  Target: nearest pedestrian, within 5m, in front
  Condition: Target in @Elevator
  Response: set velocity to {0}
.
```

---

## C.2.6. Pedestrian Attributes

This `Condition` checks if a pedestrian's `Attribute` (§Att.) is a given value (§Att.1)

```
*attribute* is *attribute value*
```

Example:
```
Action (typeA):
  Condition: state is #scared  // Condition for the pedestrian's 'State' attribute
  Response: scale velocity 2.0
.
```

---

# Att. Attributes

Each pedestrian has what are called `Attributes`.

These attributes include:
```
Position    // Coordiante pedestrian is currently at
Velocity    // Velocity vector of pedestrian
Goal        // The current coordinate the pedestrian is heading to
State       // Pedestrians internal state
```

These `Attributes` are important for `Actions` and `Conditions`.
---

# Ex. Example

```
Action (typeA):
  Condition: state is #scared  // Condition for the pedestrian's 'State' attribute
  Response: scale velocity 2.0 // Atom affecting the pedestrian's 'Velocity' attribute
.
```

---

# Att.1 Attribute Values

Attributes each have a different value type.

```
Position: Coordinate (§V.2)
Velocity: Coordinate (§V.2)
Goal    : Coordinate (§V.2)
State   : State      (§V.3)
```

Generally, `Attribute Values` can only be compared to others of the same type, except `Coordinate` values.

`Coordinate` values can be compared with `Locations`.

Example
```
Condition: position is @aisle

// or

Condition: goal is @aisle
```

---

# E. Events

An `Event` is something that occurs during a simulation, for example an announcement or fire.

`Events` have a start `Condition` and, optionally, an end `Condition` (§C)

An `Event` can only be described once, if a `Behavior` file tries to redefine an `Event` of the same name a `Behavior Error` is thrown.

`Event` names are always preceded by a `!`

Example: 
```
!announcement
```

---

## E.1. Creating Events

`Events` a defined as follows:
```
Event:
  Name: *name*
  Start: *Condition*
  End: *Condition*    // Optional
.
```

Example:

```
Event:
  Name: example
  Start: 10 seconds after !Start
  End: 10 seconds after !example
.
```


---

## E.2. Special Events

Currently, there is only one special event that is predefined for each `Behavior`: `!Start`.

The `!Start` event fires at the start of a simulation run and is always `Occurring`.

---

## E.3. Events - General Syntax Rules

---

# V. Values

---

## V.1. Numerical Values

Anywhere there is a numerical value required the following can be used (with some exceptions):
1. `Exact Values`
2. `Range Values`
3. `Random Values`

---

### V.1.1. Float vs. Number Values

There are two kinds of `Numerical Values`:
1. `Float Values`
2. `Number Values`

The difference between the two is simple:
1. `Float Values` have a decimal value
2. `Number Values` are whole numbers

---

Example:
```
50    // Number Value, has no decimal places
50.15 // Float Value, has a decimal value
```


---

### V.1.1. Exact Values

`Exact Values` are used when the value should be a specific value

`Exact Values` can either be a `Float Value` or `Number Value`.

`Exact Values` are written as simple numerical values.

```
50    
50.15 
```


---

### V.1.2. Range Values

`Range Values` are singular random values within a given range.

When the `Behavior` is run, the value is the same for every individual.

---

### V.1.2.1 Float Value Ranges  vs. Number Value Ranges 

`Range Values` are either `Float Value Ranges` or `Number Value Ranges`

With the difference being that:
`Float Value Ranges` are any real value between the range and `Number Value Ranges` are any integer value between the range.


`Range Values` are written as follows:
```
// Number Value Range -> 1, 2, or 3
1-3
or
1 to 3

// Float Value Range -> 1.0, 1.003, 1.11, 2.3, 2.5, 3.0, etc.
1.0-3.0
or
1.0 to 3.0
```

---

Example:
```
Selector:
  Type: typeA
  Select: 10-20%
.
```

The exact percentage will be a random integer value between 10 and 20.


---

### V.1.3. Random Values

`Random Values` are very similar to `Range Values`, with a random value in a given range.

The big difference is that: `Range Values` only have one random value, `Random Values` have a random value that is different for each pedestrian.

---

### V.1.3.1 Float Random Values vs. Number Random Values

`Random Values` are either `Float Random Values` or `Number Random Values`

With the difference being that:
`Float Random Value` are any real value between the range and `Number Random Values` are any integer value between the range.


Random Values are written as follows:
```
// Number Random Value -> 1, 2, or 3 (different for each pedestrian)
random 1-3
// or
random 1 to 3

// Float Random Value -> 1.0, 1.003, 1.11, 2.3, 2.5, 3.0, etc. (different for each pedestrian)
random 1.0-3.0
// or
random 1.0 to 3.0

```

---

Example:
```
Action (typeA):
  Condition: random 5-10 seconds after !Start
  Response: set velocity {0}
  Duration: random 10-20
.
```
Each typeA pedestrian will stop randomly between 5 and 10 seconds after the start, then stay still for a random amount of time between 10 and 20 seconds.

---

## V.2 Coordinates

Coordinate Values are X, Y, Z coordinates.

They are written:
```
{ *numeric value*, *numeric value*, *numeric value* }
```

The Y and Z portions can be left out, they are defaulted to 0:
```
{0}

// is the same as

{0, 0}

// is the same as

{0, 0, 0}
```


---

## V.3 State

Each pedestrian has a `State` associated with it.

`States` are user defined, and used for `Conditions`

`States` are always preceded by a '#'.

`States` are defined as follows:
```
Pedestrian States:
  #stateA,
  #stateB,
  #stateC
.
```

Ex. Example

```
Pedestrian States:
  #happy,
  #scared,
  #waiting
.
```

\tableofcontents