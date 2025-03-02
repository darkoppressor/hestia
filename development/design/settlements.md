# Settlements

Settlements belong to a civilization
Settlements have a type
Settlements generate new people over time if they are not at maximum population and there is food in the
civilization inventory
When a settlement generates a new person, food is removed from the civilization inventory
A settlement is considered under attack if it contains any enemy people
Settlement types are city TODO add more here
Settlement types have a maximum population
TODO define effect of a settlement being over its maximum population
Settlement types have a minimum desired population
Settlement types have resource gathering rates, which determine the gather rate for each type of resource
If a settlement type has a gather rate of 0 for a resource, that resource is not allowed to be gathered in settlements
of that type
Settlement types have a settlement cost in items

Technical:

Settlements (i.e., settled areas) know their parent civilization by index
Settlements know their child people by index
