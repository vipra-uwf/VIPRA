# Behavior Examples

<details>
  <summary>
    <h1>
      Ex. Creating An Example Behavior
    </h1>
  </summary>

Here we will make a couple of example `Behaviors`.

<details>
  <summary>
    <h2>
      Ex.1. The Simplest Behavior
    </h2>
  </summary>

This example is a simple `Behavior` that causes every pedestrian to stand still for the duration of the simulation. 

```
Types:           // Types Declaration (§T.) says what types of pedestrians there are
  person
.

Selector:        // Selector (§S.) says who is of that type
  Type: person
  Select: Everyone
.

Action (person): // Action (§A.) says what people of that type do
  Response: set velocity {0}
.
```

</details>

---

<details>
  <summary>
    <h2>
      Ex.2. An Announcment Event
    </h2>
  </summary>

This `Behavior` shows `Events`, with an announcement. In this example, we will make a simple `Behavior` and add to it.

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
  Condition: !Announcement occurring  // Condition (§C.)
  Response: set velocity {0}
. 
```

In this example, an announcement will start after 10 seconds and last for 10 seconds.

While the announcement is occurring, every pedestrian will be stopped to listen.

<details>
  <summary>
    <h3>
      Ex.2.1. A bit more realistic
    </h3>
  </summary>

Everyone stopping to listen isn't very realistic, so we can change it slightly.

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

In this example, only 10% of pedestrians will actually stop to listen to the announcement. (probably still an over-estimation)

</details>

<details>
  <summary>
    <h3>
      Ex.2.2. Adding more
    </h3>
  </summary>

A black-and-white, either pedestrians stop to listen or don't, may not be as realistic as we would like.

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

In this 10% of pedestrians stop to listen, another 25% slow down to listen, and the rest behave normally.

</details>
</details>

<details>
  <summary>
    <h2>
      Ex.3. Pedestrians Interacting
    </h2>
  </summary>

This example `Behavior` has pedestrians "fighting", to show that pedestrians can interact.

```
Types:
  teamA_fan
  teamB_fan
  aggressive_fan
.

Selector:
  Type: teamA_fan
  Select: 25%
.

Selector:                   // Selector (§S.) 10% of teamA fans are "aggressive"
  Type: aggressive_fan
  Select: 10%
  From: teamA_fan
.

Selector:
  Type: teamB_fan
  Select: 15%
.

Action (aggressive_fan):
  Stimulus: target within 1.0                                  // Condition (§C.)
  Response: set velocity to {0}, set target velocity to {0}    // Atoms (§A.4.) to set self and targets velocity to zero 
  Duration: random 60-120 seconds                              // Duration (§A.3.) to have fights last a random amount of time 
  Target: nearest teamB_fan                                    // Target (§A.6.) has aggressive_fans only interact with teamB_fans 
.
```


</details>
</details>

Go To [VIPRA Behaviors](./VIPRA_Behaviors.md)