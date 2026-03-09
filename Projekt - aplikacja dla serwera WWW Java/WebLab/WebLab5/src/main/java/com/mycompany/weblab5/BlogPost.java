
package com.mycompany.weblab5;

import java.io.IOException;
import lombok.*;

/**
 * Represents a single post in the blog.
 * Each post contains a date, an author, and textual content.
 *
 * @author Bartosz Mikosz
 * @version 1.1
 */
public class BlogPost {

    /** The date of the post. */
    @Getter @Setter
    String Date;

    /** The author of the post. */
    @Getter @Setter
    String Author;

    /** The main content of the post. */
    @Getter @Setter
    String Content;
    
    @Getter
    IDCode Code;

    /**
     * Constructs a BlogPost with the given date, author, and content.
     *
     * @param Date the date of the post (e.g., "2025-10-31")
     * @param Author the author's name
     * @param Content the main text of the post
     */
    public BlogPost(String Date, String Author, String Content, String Code) throws BlogException {
        this.Date = Date;
        this.Author = Author;
        this.Content = Content;
        this.Code = new IDCode(Code);
    }

    

    /**
     * Returns the date of the post.
     *
     * @return the date as a String
     */
    //public String getDate() {
        //return Date;
    //}

    /**
     * Sets the date of the post.
     *
     * @param Date the date to set
     */
    //public void setDate(String Date) {
        //this.Date = Date;
    //}

    /**
     * Returns the author of the post.
     *
     * @return the author's name
     */
    //public String getAuthor() {
        //return Author;
    //}

    /**
     * Sets the author of the post.
     *
     * @param Author the name of the author
     */
   // public void setAuthor(String Author) {
        //this.Author = Author;
    //}

    /**
     * Returns the content of the post.
     *
     * @return the post's content as a String
     */
    //public String getContent() {
       // return Content;
    //}

    /**
     * Sets the content of the post.
     *
     * @param Content the new content of the post
     */
   // public void setContent(String Content) {
        //this.Content = Content;
    //}
}
