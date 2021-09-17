# Tara Design Doc

## C++ Style

### Comments
All documentation comments should be in Visual Studios style (three slashes, ie):
```
///<summary>
///summary of the thing
///</summary>
///<param name="thing>a paramater</param>
///<returns>what it returns</returns>
```
Visual studios will auto-generate the xml flags when inputting in 3 slashes.
All classes, their public functions and members, any free-floating functions, and (preferably) all internal private members and functions should be documented like this.

Documentation for platform-specific classes that are subclasses of platform-independant abstract classes is not nesecary.

If code is very easy to understand what is happening (ie, the functions called tell the reader), then there is little need to comment it. an example:
```
IndexBufferRef train_mesh_ib = IndexBuffer::Create(...);
train_mesh_va->SetIndexBuffer(train_mesh_ib);
```
Any code that is confusing or needs more explination should be commented. When in doubt, comment!

### Locals
All local variables should be in `snake_case`. 

### Functions
All functions use `PascalCase`, and should have descriptive names.
Documentation of what the function does should be given before the declaration.
If there are a number of overloads or similar functions defined in one area, onely one comment is needed

Function paramaters should be in `camelCase`, and also have descriptive names.

### Class Names
Class, Struct, and Enum names use PascalCase. They should be descriptive. 
Class, Struct, and Enum documentation should be in the appropreate .h file, where that Class, struct or Enum is declared. Multiple related Classes/Structs/Enums should be declared in the same .h file and defined in the same .cpp file.


All major classes should also have a reference type, defined as:

```
using ClassNameRef = std::shared_ptr<ClassName>;
```

#### Class Members
All class members should be in `PascalCase`, with a prefix of "`m_`". Again, they should be descriptive. The vast majority of members should be private, with Getters and Setters, following the convention:
```
private:
	float m_ThisVar;
public:
	inline float GetThisVar() const {return m_ThisVar;}
	inline void SetThisVar(float thisVar) {m_ThisVar = thisVar;}
```

#### Static members
All static members of classes, files, or functions should be in `PascalCase`, with a prefix of "`s_`".


#### Class Functions
Like regular functions, they are in `PascalCase`, and paramaters are in `camelCase`.
class methods that override virtual class members should have both the `virtual` keyword and the `override` keyword in their declaration. This helps us the programmers see what is going on easier.

## Code Usage Style

### C++ Standard Template Library usage
The C++ Standard Template Library should be used. No need to re-invent the wheel

### Graphics- and Platform-specific abstractions
All platform or graphics specific code should be behind a platform-independant abstraction. The abstract class should have a static Create(...) function, that checks the chosen backend to make (using platform/architexture macros if nesecary) and returnes a reference type of that type (ie, ClassNameRef)
	There is the possiblity that that class (like Window) should only ever have ONE type of subclass that can be made, in that case, the Create(...) function should be defined in the .cpp file of the platform-specific subclass.
Platform-specific subclasses of that type should exist in the Platform directory, under the specific platform.

### C++ numerical types
instead of using `int` or `unsigned int`, stick to the standard, guaranteed sized int types, `int32_t`, `uint32_t` and the like, for 8, 16, 32, and 64 bit numbers.

### Ref types
As mentioned in the Class area, most classes should have an associated Ref type that is a typedef (technically, `using`) of a `std::shared_ptr<Class>`. These should be used primarily instead of the raw class.

Non-owning pointers should ALWAYS be raw pointers. When passing ownership, either use a move constructor, or, if you are passign ownership of a new item, pass as raw pointer.



