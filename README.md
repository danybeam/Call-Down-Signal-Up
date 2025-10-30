# Call-Down-Signal-Up

## Read BEFORE jumping into the code

This repository is a toy repo to explain what I call "the chain".
The chain is is a basardisation of Godot Engine's phylosophy of "Call down signal up" (CDSU)

Note 1:
> This document obviates the public/protected/private markers.  
> They are mostly irrelevant for the conversation but the example code in engine will have them.

Note 2:
> The example code here and the example code in engine might differ slightly.  
> This will be cause either because I'm simplifying the concepts or I could not remember the exact syntax in the moment.  
> When in doubt trust the in-engine code. This document is more conceptual.

### The Principle in Plain English

The idea behind CDSU is that code dependencies only go one way.
If you have a farm that manages chickens and those chickens lay eggs the dependecy chain is

``` text
Farm -depends on-> Chickens -depends on-> Eggs
```

A farm needs to know how many chickens it has but the chicken doesn't know nor care if it's on a farm.  
A Chicken needs to know how to lay an egg but eggs should not care about which chicken laid them.

#### Shouldn't the chicken depend on the farm?

In real life, yes. In code we call "dependencies" code that needs to be called for other code to work.  
e.g.

```C++
class Farm
{
    TArray<Chicken> ChickensInFarm;
}
```

in this case the class `Farm` **must** know what a `Chicken` looks like to work properly. Therefore `Farm` **depends** on `Chicken`

### The Principle in Code Terms (or "Why do we bother?")

Using the previous example

``` text
Farm -depends on-> Chickens -depends on-> Eggs
```

and modifying it a little bit

```C++
class Farm
{
    TArray<Chicken*> ChickensInFarm;
    void PrintFirstChickenName();
}
```

that means that doing this is allowed:

```C++
void Farm::PrintFirstChickenName()
{
    Chicken* firstChicken = ChickensInFarm[0];
    cout << firstChicken->GetName() << endl; // <--- Notice how we called the GetName() method directly on the chicken
}
```

However, under CDSU this is not allowed:

```C++
// Chicken class definition doesn't matter yet

void Chicken::Cluck()
{
    Farm* myFarm = GetFarm();

    myFarm->AnnounceIClucked(this); // <--- FORBIDDEN: because Farm depends on chicken and chicken depends on Farm.
}
```

Having Farm depend on Chicken and Chicken depend on Farm is what is called a circular dependency.  
Circular dependencies are very bad. The main problem is that they can augment exponentially the number of classes that need to be checked/tested/fixed if one of the classes changes. Additionally depending on how you write your `#include` statements your compilation may even fail.  
Following with the example above, if `Farm::AnnounceIClucked()` changes and takes more parameters, `Chicken::Cluck()` is forced to change to acomodate that.

### CDSU in Practice

"Let's say you convinced me, how do we solve that problem?" Excelent question!  

We change Farm and Chicken to look like this

Farm:

```C++
// Farm.h
class Farm
{
    Farm();
    TArray<Chicken*> ChickensInFarm;
    void PrintFirstChickenName();
    void OnChickenRegistration(Chicken* newChickenPtr);
}

// Farm.cpp

Farm::Farm()
{
    Chicken::RegistrationDelegate.AddUnique(this, &Farm::OnChickenRegistration); // NOTICE the delgate registration. this is half of the Signal Up part of CDSU
}

// Print first chicken name doesn't change from previous examples.
// In case it wasn't obvious, calling firstChicken->GetName() is the Call Down part of CDSU

void Farm::OnChickenRegistration(Chicken* newChickenPtr)
{
    ChickensInFarm.Add(newChickenPtr);
}
```

Chicken:

```C++
// Chicken.h
DECLARE_DELEGATE_ONE_PARAM(FChickenRegistrationDelegate, Chicken*, chickenToRegisterPtr)

class Chicken
{
    Chicken(); // In engine this BeginPlay would probably be a better place for what I'll do here.
    
    // This delegate being static is not inherently part of CDSU
    // It's a quirk of this specific example but the principle should work the same.
    static FChickenRegistrationDelegate RegistrationDelegate; 
}

// Chicken.cpp

Chicken::Chicken()
{
    RegistrationDelegate.Broadcast(this); // NOTICE this is the other half of the Signal Up part of CDSU
}
```

### Ok but seriously, why bother?

Notice how the `Chicken` class has no references to `Farm` it doesn't know that it is in a farm, it doesn't care that it is in a farm, it doesn't even care if anyone catches their `RegistrationDelegate` broadcast.  
`Farm` can change as much as we want and there is no concerns that it may accidentally break `Chicken`.  
Even more, this makes it so that if we want to register a `Chicken` on a `Field` or `SomeDudesBackyard` the chicken still doesn't care.  
The chicken tells its dependents "You want to know when I want to register? You MUST subscribe to this delegate. No two ways about it."  

---

Now you can jump into the code