-Train Booking System,

You have been hired by the IRCTC (sad) to design a Train Booking System. The System should work as specified below:

-High-Level Overview,

1. Login In and Sign Up
(i)The system should be accessed via a CLI (Command Line Interface)
(ii)It should consist of a Passenger "Login" (List the Passengers / Admin)
(iii)A login (applicable for both passenger and admin) should consist of a unique username/id. Enabling security measures (like passwords) is optional
(iv)A new user should also be able to sign up into the system and provide a valid (non-empty) Name, Username, and Password

2.What should a Passenger(P) be able to do
(i)Book a Ticket from between 2 locations say (A and B) on a desired date
(ii)A booking can only result in a single ticket being booked
(iii)Upon booking a ticket, P should be assigned a ticket containing a Seat and Coach Number
(iv)Upon booking the ticket the User should be made aware of the cost of the Ticket (could be calculated from either the Euclidean or Manhattan distance of the two locations)
(v)Between Two Locations, on a given date, there is a single train going between them
(vi)Each Train has only 2 Coaches with 4 seats on each coach
(vii)No two passengers can have the same (start_location, end_location, date, coach, seat) value
(viii)A passenger should not be allowed to book a ticket on the same date
(ix)If a train is full, the passenger should immediately be notified that they have not been allotted a ticket and cannot do so

-Low-Level Overview,

(i)Choose appropriate access specifiers (public, private, and protected) for the various attributes
(ii)Given below are the class diagrams for the classes that you have to implement
(iii)Do not restrict yourself to these, they are just to get you started and help you get an idea
