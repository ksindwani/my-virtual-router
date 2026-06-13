# my-virtual-router

## My Thinking and Approach
1. Initially started with giving the prompts specified in `AI_USAGE.md` (provided initial prompts in specified readme and detail prompts and solutions further in the file `router-chat.docx`)
2. Those prompts were provided to give LLM an idea of what is the problem statement and getting initial code solution.
3. Fixed json specific, routing bugs specific to interface state not being read and was facing best match path incorrect etc.. with self debugging and using AI
4. Created test cases `(test_cases.md)` and validated routing logic
5. Until step 4 I was able to complete the implementation and validation was succesfull for static routing cases
6. Then I started implementing dynamic routing, where the change in interface state will be read dynamically from input json files and routing logic and output will work accordingly
7. I am thinking of 2 approaches to implement dynamic routing:
  7.1. Parse the input json file at a specific time interval
  7.2. Notify mechanism where once the json files gets modified, router will be notified to trigger a reload

## Compilation Steps
1. Execute `make` inside "my-virtual-router" project directory
2. `router-basic` executable will be created

## Execution Steps/Commands
```
./router-basic show-routes
./router-basic show-interfaces
./router-basic lookup <dest-ip>
./router-basic explain-lookup <dest-ip>
```
