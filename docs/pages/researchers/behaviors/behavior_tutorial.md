
| [Main Page](../../../docs.md) | [How To Use](../usage.md) | [Provided Modules](../base_modules.md) | [VIPRA Behaviors](../behaviors/behaviors.md) | [Module Parameters](parameters.md) | [Developing Modules](../../developers/overview.md) |

# VIPRA Behavior Tutorial

§ markers point to the [Behavior Syntax Reference](behaviors_syntax.md)

# Ex. Creating An Example Behavior

Here we will make a couple of example `Behaviors`.

## Ex.1. The Simplest Behavior

This example is a simple `Behavior` that causes every pedestrian to stand still for the duration of the simulation. 

<details open>
<summary>Code</summary>

```
Types:           // Types Declarations (§T.) say what types of pedestrians there are
  person
.

Selector:        // Selectors (§S.) set how pedestrians are chosen to have the type
  Type: person
  Select: Everyone  // in this case everyone is selected
.

Action (person): // Actions (§A.) decide what pedestrians of the given type do
  Response: set velocity {0}  // in this case they are always stopped
.
```
</details>

---

## Ex.2. An Announcment Event

This `Behavior` shows `Events`, with an announcement. In this example, we will make a simple `Behavior` and add to it.

<details open>
<summary>Code</summary>

```
Types:                                // Types Declaration(§T.)
  Listener
.

Selector:                             // Selector (§S.)
  Type: Listener
  Select: Everyone
.

Event:                                // Event (§E.)
  Name: Announcement
  Start: 10 seconds after !Start      // Start Condition (§C.)
  End: 10 seconds after !Announcement // End Condition (§C.)

.

Action (Listener):                    // Action (§A.)
  Condition: !Announcement occurring  // Condition (§C.) sets what causes the pedestrian to do this action
  Response: set velocity {0}
. 
```
</details open>

In this example, an announcement will start after 10 seconds and last for 10 seconds.

While the announcement is occurring, every pedestrian will be stopped to listen.


### Ex.2.1. A bit more realistic

Everyone stopping to listen isn't very realistic, so we can change it slightly.

<details open>
<summary>Code</summary>


```
Types:                                // Types Declaration(§T.)
  Listener
.

Selector:                             // Selector (§S.)
  Type: Listener
  Select: 10%
.

Event:                                // Event (§E.)
  Name: Announcement
  Start: 10 seconds after !Start      // Start Condition (§C.)
  End: 10 seconds after !Announcement // End Condition (§C.)

.

Action (Listener):                    // Action (§A.)
  Condition: !Announcement occurring  // Condition (§C.)
  Response: set velocity {0}
. 
```
</details>
In this example, only 10% of pedestrians will actually stop to listen to the announcement. (probably still an over-estimation)

### Ex.2.2. Adding more

A black-and-white, either pedestrians stop to listen or don't, may not be as realistic as we would like.

<details open>
<summary>Code</summary>


```
Types:                                // Types Declaration(§T.)
  Listener
  Partial_Listener
.

Selector:                             // Selector (§S.)
  Type: Listener
  Select: 10%
.

Selector:                             // Selector (§S.)
  Type: Parital_Listener
  Select: 25%
.

Event:                                // Event (§E.)
  Name: Announcement
  Start: 10 seconds after !Start      // Start Condition (§C.)
  End: 10 seconds after !Announcement // End Condition (§C.)

.

Action (Listener):                    // Action (§A.)
  Condition: !Announcement occurring  // Condition (§C.)
  Response: set velocity {0}
.

Action (Parital_Listener):            // Action (§A.)
  Condition: !Announcement occurring  // Condition (§C.)
  Response: scale velocity 0.75
.
```
</details>

In this 10% of pedestrians stop to listen, another 25% slow down to listen, and the rest behave normally.