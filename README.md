# CPP-Design-Patterns
Tasks for AV Soft internship.


# Comments for Task 2 final commit
1) In final commit, I got rid of "stupid" pointers and made all the mutations using mutable specifier.
Also, I fixed the issue with memory loss in Invoker. Pointers there are required due to the mechanism of virtuality in C++. So they got freed in the destructor properly.
2) I fixed the issue with looping over the same command if input was incorrect.
