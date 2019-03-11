### How compilation and works

It normally invokes the preprocessor, which is dedicated to dealing
with #define and #includes. Then it invokes what is truly called the compiler. That’s what’s responsible for taking your `.c` files, and your `.cc` files, and generating all those `.o` files, that happen to appear in your directory, after you type make. 


They all make compilation look like it’s the full effort to **create an executable**. But once all those `.o` files are created, and one of the `.o` files has code for the main function inside, there is
this step called ** **, which is responsible for taking a list of `.o` files; stacking them on top of one another; making sure that any function call that could possibly ever happen during execution, is accounted for because there’s code for that function one of the `.o` files.


---


### Preprocessor

#### `#define`

```C
#define kWidth 480
#define kHeight 720

printf("Width is %d\n", kWidth);
int area = kWidth * kHeight;
```


When you feed, or when a `.c` file is fed to GCC or G++, there’s a component called the **preprocessor** that doesn’t do very much in terms of compilation. It doesn’t do compilation. It actually reads the `.c` file that’s fed to it, from top to bottom, left to right, like
we would read a page. 

And every time it sees a `#define`, it really only pays attention to these words. What it’ll normally do is, it’ll read in a line. And if there’s nothing that has a # at the beginning of the line, it’ll just say, “Okay, I just read this one line, and I don’t care about it. 

It internally has something like a hash set. It associates this as a key with this string of text; it doesn’t even really know that it’s a number. It just says, “Everything between the white space that comes right after width, and the white space that comes after the first token, that’s associated as a text stream with that identifier. 

<br>

```C
printf("Width is %d\n", 480);
int area = 480 * 720;
```
It is really text, at the moment – it’s just that, when it’s fed to the next phase of compilation, it’ll go ahead, and it’ll chew on those things, and recognize that they really are numbers. And that’s when it might do the multiplication.