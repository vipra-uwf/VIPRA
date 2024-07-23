# Adding Behaviors

Behavior files are held under `Behaviors/`.

To use behaviors in simulations add them to `module_params.json` under `human_behavior_model`.

Example:
```
// Using the "example.behavior" Behavior file

"human_behavior_model": {
    "seed": 4525,
    "behaviors": [
      "example"       // Add behavior name here without ".behavior"
    ]
},
```