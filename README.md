# knowing

Here's the basics:  
  To make a database file:  
    Come up with all the topics that should be in the database. Figure out which ones depend on which ones. Then write a regular ascii file, where you have it formatted as follows:  
    prerequisite topic,prerequisite topic 2,prerequiste topic 3#topic  
    some prerequisite,other prerequisite#prerequisite topic 3  
    #A topic  
    You can have any non-negative number of prerequisite topics (0 is valid!) followed by the topic that depends on that.  
    If you have Topic A that requires you to understand Topic B that requires you to understand Topic C, you don't need to put Topic C in Topic A's prerequisites. It is a waste of time and it doesn't matter if you do it.  
    So the file would look like this:  
    Topic B#Topic A  
    Topic C#Topic B  
    Notice that I don't make a Topic C entry. That's because the prerequisite topics are automatically included as topics. You don't need to also have a line that says #Topic C  
    
    
  To make a student file:  
    This one's simple! Just type in each topic the student knows, and hit enter in between each one! Like this:  
    cheese curd manufacturing  
    Topic C  
    prerequisite topic 2  
    You don't need to list any of the prerequisite topics to a topic you know. If you understand a topic, you must understand the prerequisites (or else the database file is wrong! This is a realistic possibility because sometimes we overestimate what people need to know in order to learn something) so the program assumes that you know them.  

You can also use this program to help figure out what courses you can take given which ones you've taken, but I haven't implemented co-requisites as a feature, so be aware of that.

If there's anything else you want to know you can get in touch with me over Google+, I'm Mack Colwell there.
