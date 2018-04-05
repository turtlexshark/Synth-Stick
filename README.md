# Synth-Stick
I have no idea if this is going to compile because I wrote it on a text editor at work, but I took a stab at fixing the machine gun firing of notes that we were experiencing. Here are the changes I made:

- I divided our SoftPot into 15 sections and set the notes to play a Major scale from E to the E two octaves up.
- I made an enum for the "fret" that is currently being played.
- I made a variable to store what "fret" was played before and a variable for the current "fret"
- I made a function that sets the enum variables to their correct values
- Before entering the if else of notes to play, the program now checks to see if the current note to play is the same as the note played previously. If it is, the if else of notes to play is skipped
