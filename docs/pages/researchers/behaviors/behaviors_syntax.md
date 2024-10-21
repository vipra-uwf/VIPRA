
| [Main Page](../../../docs.md) | [How To Use](../usage.md) | [Provided Modules](../base_modules.md) | [VIPRA Behaviors](../behaviors/behaviors.md) | [Module Parameters](parameters.md) | [Developing Modules](../../developers/overview.md) |

# VIPRA Behaviors Syntax Reference

>**Document Note:** Text wrapped with `<` and `>`, denote a placeholder value. You should not include `<` and `>` when writing Behaviors.*

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
<declaration_type>:
  <component_name> : <component_value>
  ...
.
```

Each section goes over what components are available for each.

Example:
```
Selector:
  Type: injured_person
  Select: 10%
.
```
`Type` and `Select` are both components of the [`Selector`](#selectors) declaration

---

# Behavior General Syntax Rules

This section has some simple `Syntax` rules that must be followed for a `Behavior` to be considered correct.

The other sections will have the specific `Syntax` rules for their respective parts, in a `Section - General Syntax Rules` section

1. All Declarations must end in a `.`
```
correct:
  Event:
    Name: Example
    ...
  .

incorrect:
  Event:
    Name: Example
    ...
```

2. Each `Behavior` must **start** with a [Types Declaration](#types-declaration)
3. Each `Behavior` must **only have 1** [Types Declaration](#types-declaration)
4. Each `Behavior` must have **at least 1** [Selector](#selectors)
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
  Start: <start_condition>
.

Event:
  Start: <start_condition>
  Name: Example
.
```

---

# Types

`Types` are how pedestrians are organized in `Behaviors`

Each pedestrian is assigned a user-defined `Type`. These `Types` are used to categorize the [Actions](#actions) pedestrians will take during the simulation.

---

## Types Declaration

`Types` are declared in the `Types Declaration` of a `Behavior`.

A `Types Declaration` is what says which types are being used in the `Behavior`.

`Behaviors` can only have 1 `Types Declaration` which can include up to 64 `Types`.

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

### Types Declaration General Syntax Rules

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

## Composite Types

A Pedestrians `Type` can be composed of several other types.

A Pedestrian with a `Composite Type` will have the attributes of each individual `Type`.

How to assign `Composite Types` is explained in the [Selectors](#selectors) section.


---

## Groups

For most users, `Groups` and `Types` refer to the same thing.

The main difference between a `Type` and a `Group` is that there is a base `Group` for each `Behavior`, being 'Pedestrian' (or 'Pedestrians').

Each pedestrian with a `Type` is considered in that `Type`'s `Group`.

Pedestrians with `Composite Types` are in a `Group` for each `Type`.


---

## Type General Syntax Rules

1. `Type` names can only contain Letters, Underscores, and Hyphens `(a-z), (A-Z), '_', '-'`


---


# Locations

`Locations` are rectangular regions that can be used in `Selectors`, `Conditions`, and `Actions`.

After being declared, `Location` names are always preceded by a `@`

```
Location:
  Name: <name>
  Dimensions:
    Center: <center_coords>  // Coord Value
    Lengths: <side_lengths>  // Coord Value
    Rotation: <rotation>     // Numeric Value
.
```
- \*Name* - The name used to reference this `Location`
- \*Dimensions* - The dimensions for the `Location`
  - \*Center* - X,Y,Z coordinates for the center of the rectangle as [coordinates](#coordinates).
  - \*Lengths* - X,Y,Z lengths of the rectangle as [coordinates](#coordinates).
  - \*Rotation* - degrees of clockwise rotation as a [numeric value](#numerical-values)


Example:
```
Location:
  Name: Aisle
  Dimensions:
    Center: {15, 1.7}     // Center of rectangle is at (15, 1.7)
    Lenghts: {30, 0.3}    // Width of 30m, Height of 30cm
    Rotation: 0           // Rotated 0 degrees (aligned with X axis)
.
```
---

## Locations General Syntax Rules

1. `Location` names are always preceded by a `@` after the declaration

---

# Selectors

`Selectors` are how pedestrians are selected for a certain [Type](#types).

Selecting pedestrians is done through a `Selector` declaration. The basic syntax is as follows:
```
Selector:
  Type: <types>
  From: <type>                  // Optional, defaults to base pedestrians group
  Select: <selection_criteria>
.
```

- \*Type* - The [Type](#types) to assign to selected pedestrians.
- \*From* - The [Group](#groups) to select pedestrians from.
- \*Select* - The exact `Select` criteria to use. Available `Select` criteria are listed under [Available Select Criteria](#available-select-criteria).

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

## Selecting From Groups

`Selectors` can select from specific [Groups](#groups)

This has the effect of selecting the pedestrians for a [Composite Type](#composite-types) but allows for more dynamic proportions. This allows for refined selections within selections. 


```
Types:
  typeA
  typeB
  typeC
.

Select:        // Selects 50% of pedestrians to be typeA
  Type: typeA
  Select: 50%
.

Select:        // Selects 15% of typeA pedestrians to also be typeB
  Type: typeB
  From: typeA
  Select: 15%
.

Select:       // Selects 5% of typeA pedestrians to also be typeC
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

## Selector Exclusivity

When a pedestrian is selected from a `Group`, it is marked as used and can not be selected by another `Selector`.

```
Select:
  Type: typeB
  Group: typeA
  Select: 15%
.

Select:
  Type: typeD
  Group: typeA
  Select: 20%
.
```
Means:
Those 15% of 'typeA' pedestrians that also are typeB can not be selected from again. Any selections for typeD in the above scenario will never also be typeB. 

However, they can be selected **from** 'typeB'.

TODO(tyler): What happens if those values exceed 100%?

```
Select:
  Type: typeC
  Group: typeB
  Select: 15%
.
```

This may result in some pedestrians being of types typeA, typeB, and typeC.

TODO(tyler): What happens if we want some overlap?

---

## Required Selectors

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

## Available Select Criteria

1. [Everyone](#everyone)
2. [Percent](#percent)
3. [Exactly N](#exactly-n)

---

### Everyone

```
Select: Everyone
```

This select criteria will select every pedestrian to have the selected [Type](#types)


---

### Percent


Selects a percentage of a `Group` for the provided `Type`

```
<value>%
```
\<value\> - [Number Value](#numerical-values) (1 - 100)

 
Example:
```
Select: 15%
```


---

### Exactly N 

Selects an exact number of pedestrians from a `Group`.

```
Exaclty <value>
```
\<value\> - [Number Value](#numerical-values)

 
Example:
```
Select: 10
```


---

### Location

Selects all pedestrians inside a given location.

```
Select: In <location>
```
\<location\> - Name of [Location](#locations)

Example:
```
Select: In @Elevator
```

---

## Selectors General Syntax Rules

---


# Actions

`Actions` are what affect a Pedestrian's position, velocity, state, etc.

`Actions` work by applying attribute modifiers known as [Atoms](#atoms) each time step their `Condition` is satisfied.

---

## Unconditional Actions

An `Unconditional Action` will ALWAYS take effect, applying its `Atoms` each timestep.


```
Action (<type>):
  Response: <atoms>
.
```
- \<type\> being the pedestrian type that follows this `Action`
- \<atoms\> being the steps taken in an action, more in *[Atoms](#atoms)*


Example:
```
Action (injured_person):         // Action Declaration for "injured_person"s
  Response: scale velocity 0.75  // "Scale" Atom
.
```
With this Behavior, an injured person will always walk at 75% speed.


---

## A.2. Conditional Actions

A `Conditional Action` will only take effect if its `Condition` is satisfied.

`Conditional Actions` are written as follows:
```
Action (<type>):
  Condition: <condition>
  Response: <atoms>
.
```

- \<type\> being the pedestrian type that follows this `Action`. *See [Types](#types)*.
- \<atoms\> being the steps taken in an action. *See [Atoms](#atoms)*.
- \<condition\> being the condition to be filled. *See [Conditions](#conditions)*. 

---

Example:
```
Action (listener): 
  Condition: !announcement occurring. // Condition
  Response: set velocity to {0}          // Atom
.
```
Pedestrians of listener will stop moving while the announcement event is occurring.


---

## Action Durations

Normally `Actions` only apply to the time step their `Condition` is true in.

If a longer response is needed, a `Duration` can be added to the `Action`.

```
Action (<type>): 
  Condition: <condition>
  Response: <atoms>
  Duration: <value>
.
```
- \<type\> being the [Type](#types) the action applies to
- \<atoms\> being the [Atoms](#atoms) for the `Action`
- \<condition\> being the [Condition](#conditions) for the action to start
- \<value\> being a numerical [Value](#values), for how long the action should continue

---

Example, Someone Tripping:
```
Action (tripper):
  Condition: 10 seconds after !Start // Condition
  Response: set velocity to {0}         // "Set" Atom
  Duration: 15 seconds               // Duration
.
```

Pedestrians of type `tripper` will trip after 10 seconds from the simulation starting, and remain still for 15 seconds.

This will cause all `trippers` to trip at exactly the same time. Normally, randomness should be added to a scenario such as this. For more information on how to add randomness to `Duration` and `Conditions`, see *[Random Values](#random-values)*.


---

## Atoms

`Atoms` are the part of an `Action` that actually affects a pedestrian.

The do this by changing pedestrian [Attributes](#attributes).

See *[Available Atoms](#available-atoms)* for the list of available `Atoms`.

---

### Atom Lists

An `Atom List` is the collection of `Atoms` that make up an `Action`.

`Atom Lists` are written as follows:
`<atom> , <atom>`

**Important Note:** `Atom` effects are applied in the order they are listed

---

Example:

```
set velocity to {0}, set state to #listening
```
This `Atom List` will result in the pedestrian stopping and their state being set to "listening"

(Pedestrian state is useful in [Conditions](#conditions))


---

## Available Atoms

There are two types of available `Atoms`:
1. [Set](#set)
2. [Scale](#scale)
---

### Set

The `Set Atom` sets a pedestrian's attribute to a specific value.

```
set <attribute> to <attribute_value>
```

- \<attributes\> are described in [Attributes](#attributes).
- \<attribute_values\> are described in [Attribute Values](#attribute-values).


### Scale

The `Scale Atom` multiplies a pedestrian attribute by a given value

```
scale <attribute> <attribute_value>
```

Currently, only pedestrian `Velocity` can be scaled.

- \<attributes\> are described in [Attributes](#attributes).
- \<attribute_values\> are described in [Attribute Values](#attribute-values).


---

## Action Targets

Sometimes Pedestrians will want to affect other pedestrians around them.

This is done through the use of `Targets`.

```
Action (<type>):
  Condition: <condition>  // Condition
  Response: <atoms>       // Atoms
  Target: <target>        // Action Target
```

---

Example:

```
Action (aggressive):
  Response: set target velocity {0}
  Target: nearest pedestrian
```
This results in the pedestrian stopping the nearest pedestrian around them.


---

### Available Targets

There is currently only one type of action `Target`, the "nearest" `Target`. This `Target` describes proximity to a pedestrian as part of an `Action`.

---

#### Nearest

Selects the nearest of a given type to be the target

```
nearest <type_list>   // selects the nearest pedestrian with any of the listed types

// or

nearest pedestrian    // selects any pedestrian
```
  
---

### Target Modifiers

`Target Modifiers` change how pedestrians are selected to be the Target of an action.

`Target Modifiers` are simply appended to the end of the `Action Target`.

Example:
```
Target: nearest pedestrain in front within 1.0
```
This selects the nearest pedestrian in the direction of motion that is within 1 meter.

---

#### Distance Target Modifier

Targets can be filtered by their distance to the pedestrian.

```
within <numeric_value>
```

---

#### Direction Target Modifier

Targets can be filtered by their direction from the pedestrian.

Currently, there are only two directions.
```
in front   // used to select pedestrians in the direction the pedestrian is headed

behind     // used to select pedestrians in the opposite direction the pedestrian is headed
```

---

#### Location Target Modifier

Targets can be filtered by whether they are inside a `Location` or not.

```
in <location_name>
```

---

# Conditions

A `Condition` is what decides if a `Conditional Action` or `Event` occurs. 

`Conditions` are comprised of one or more `Sub Conditions` and the boolean operations between them (and/or).

`Conditions` do not stand on their own, and only appear as part of an `Action` or `Event` declaration.

---

Example:

```
Action (typeA):
  Condition: !example occurred and 5 seconds after !Start
  Response: set velocity to {0}
  Duration: 10 seconds
.
```
This results in typeA pedestrians stopping for 10 seconds after the example event has occurred but not before 5 seconds have passed in the simulation.


---

## Sub Conditions

A `Sub Condition` defines a single condition of the state of the simulation in which it returns true.

*See [Available Sub Conditions](#available-sub-conditions)*


---

## Available Sub Conditions

There are currently 6 total available sub conditions:
1. [Elapsed Time](#elapsed-time)
2. [Spatial](#spatial)
3. [Event Status](#event-status)
4. [Location Enter/Leave](#location-enterleave)
5. [Inside Location](#inside-location)
6. [Pedestrian Attributes](#pedestrian-attributes)

---

### Elapsed Time

This `Condition` will be true when a provided amount of time has passed from the **start** of an [Event](#events)

**Note:** This is true only for **one time step** when the time has elapsed, until the `Event` starts again.

```
<numeric_value> seconds after <event>.
```

---

Example:
```
Action (typeA):
  Condition: 10 seconds after !Start // Elapsed Time Condition
  Response: set velocity to {0}         // "Set" Atom
  Duration: 5 seconds                // Duration
.
```
**Note:** without the `Duration` the pedestrian would stop moving for only one time step. *See [Duration](#action-durations)*.


---

### Spatial

This `Condition` will be true when the condition Target is within a certain distance

```
target within <numeric_value>.
```

---

Example:
```
Action (typeA):
  Condition: 10 seconds after !Start // Elapsed Time Condition
  Response: set velocity to {0}         // "Set" Atom
  Duration: 5 seconds                // Duration
.
```
**Note:** without the `Duration` the pedestrian would stop moving for only one time step. *See [Duration](#action-durations)*.


---

### Event Status

This `Condition` checks for the status of an [Event](#events).

```
<event> is <status>
```

#### Available Statuses:
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

## Location Enter/Leave

This `Condition` checks if a pedestrian has entered or left a [Location](#location).

```
Enter <location_name>

// or

Exit <location_name>
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

## Inside Location

This `Condition` checks if a pedestrian is inside a [Location](#location).

```
In <location_name>
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

## Pedestrian Attributes

This `Condition` checks if a pedestrian's [Attribute](#attributes) is a given [Attribute Value](#attribute-values).

```
<attribute> is <attribute_value>
```

Example:
```
Action (typeA):
  Condition: state is #scared  // Condition for the pedestrian's 'State' attribute
  Response: scale velocity 2.0
.
```

---

# Attributes

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

## Attribute Values

Attributes each have a different value type.

```
Position: Coordinate
Velocity: Coordinate
Goal    : Coordinate
State   : State
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

# Events

An `Event` is something that occurs during a simulation, that effects the whole simulation. For example, an announcement or fire could be an `Event`.

`Events` have a start `Condition` and, optionally, an end [Condition](#conditions)

An `Event` can only be described once, if a `Behavior` file tries to redefine an `Event` of the same name a `Behavior Error` is thrown.

`Event` names are always preceded by a `!`

Example: 
```
!announcement
```

---

## Creating Events

`Events` a defined as follows:
```
Event:
  Name: <name>
  Start: <condition>
  End: <condition>    // Optional
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

## Special Events

Currently, there is only one special event that is predefined for each `Behavior`: `!Start`.

The `!Start` event fires at the start of a simulation run and is always `Occurring`.

---

## Events General Syntax Rules

`Event` names are always preceded by a `!` when used.

---

# Values

`Behaviors` support 3 types of values:

1. [Numeric](#numerical-values)
2. [Coordinates](#coordinates)
3. [State](#state)

---

## Numerical Values

Anywhere there is a numerical value required the following can be used (with some exceptions):
1. [Exact Values](#exact-values)
2. [Range Values](#range-values)
3. [Random Values](#random-values)


---

### Exact Values

`Exact Values` are used when the value should be a specific value

There are two kinds of `Exact Values`:
1. `Float Values`
2. `Number Values`

`Float Values` have decimal precision, while `Number Values` are only integers. 

`Exact Values` are written as simple numerical values.

Example:
```
50    // Number Value, has no decimal places
50.15 // Float Value, has a decimal value
```


---

### Range Values

`Range Values` are singular random values within a given range.

When the `Behavior` is run, the value is the **same for every individual using that `Value`**.

---

#### Float Value Ranges  vs. Number Value Ranges 

`Range Values` are either `Float Value Ranges` or `Number Value Ranges`

With the difference being that:
`Float Value Ranges` are any real value between the range and `Number Value Ranges` are any integer value between the range.


`Range Values` are written as follows:
```
// Number Value Range -> 1, 2, or 3
1 to 3

// Float Value Range -> 1.0, 1.003, 1.11, 2.3, 2.5, 3.0, etc.
1.0 to 3.0
```

---

Example:
```
Selector:
  Type: typeA
  Select: 10 to 20%
.
```

The exact percentage will be a random integer value between 10 and 20.


---

### Random Values

`Random Values` are very similar to `Range Values`, with a random value in a given range.

The big difference is that: `Range Values` only have one random value, `Random Values` have a random value that is **different for each pedestrian**.

---

#### Float Random Values vs. Number Random Values

`Random Values` are either `Float Random Values` or `Number Random Values`

With the difference being that:
`Float Random Value` are any real value between the range and `Number Random Values` are any integer value between the range.


Random Values are written as follows:
```
// Number Random Value -> 1, 2, or 3 (different for each pedestrian)
random 1 to 3

// Float Random Value -> 1.0, 1.003, 1.11, 2.3, 2.5, 3.0, etc. (different for each pedestrian)
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

## Coordinates

Coordinate Values are X, Y, Z coordinates.

They are written:
```
{ <numeric_value>, <numeric_value>, <numeric_value> }
```

**The Y and Z coordinates are optional. When not specified, these coordinates are considered 0.**

Example:
```
{0}

// is the same as

{0, 0}

// is the same as

{0, 0, 0}
```


---

## State

Each pedestrian can have a single `State` associated with it. When a new state is set, it overrides the previous state. 

`States` are user defined, and used for `Conditions`

`States` are always preceded by a '#' when used, but are excluded in the definition.

`States` are defined as follows:
```
States:
  stateA,
  stateB,
  stateC
.
```

Example:
```
States:
  happy,
  scared,
  waiting
.
```