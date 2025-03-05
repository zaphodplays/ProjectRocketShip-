This RocketShip project potrays the functionality of assembling multiple RocketStage actors to integrate them into the RocketShip actor using static mesh welding to 
enable a single Rigid body dyanamics for the attached bodies. 
RocketShipAssemblyComponent is used by the Player Controller to assemble the RocketStage actors. 
RocketShip actor has the following functional components - 
1. ShipAbilityComponent - GameplayAbilitySystem component that manages the state behavior of the Ship by triggering relevant GameplayAbilities. 
2. ShipStagingController - manages the staging behavior of the Ship
3. RocketThrustComponent - applies Thrust Vector to the Ship body
All this while playing in a multiplayer scenario
