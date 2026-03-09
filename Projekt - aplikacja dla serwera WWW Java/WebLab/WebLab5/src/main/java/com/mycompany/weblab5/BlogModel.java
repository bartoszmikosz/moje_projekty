package com.mycompany.weblab5;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Scanner;
import lombok.Getter;
import lombok.Setter;

/**
 * Represents the model component of the blog application.
 * It stores the collection of blog posts and admin mode flag.
 * Handles all business logic: adding, removing, editing posts and file I/O.
 * 
 * @author Bartosz
 * @version 1.3
 */
public class BlogModel {

    /** Integer flag indicating whether the user is an admin (1) or guest (0). */
    @Getter @Setter
    private int adminmode = 0;

    /** List storing all blog posts created during runtime. */
    @Getter
    private final List<BlogPost> blogList = new ArrayList<>();

    private static final String SEPARATOR = "-?/#-?/#-?/#";
    

    private String fileName;
    
     // setter tylko do testów
    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    /**
     * Generates a random 5-letter lowercase string used as unique post ID.
     *
     * @return String consisting of 5 random lowercase letters.
     * @throws BlogException if the generated ID does not have length 5.
     */
    public String RandomLetters() throws BlogException {
        Random rand = new Random();
        StringBuilder sb = new StringBuilder(5);
        String letters = "abcdefghijklmnopqrstuvwxyz";

        for (int i = 0; i < 5; i++) {
            sb.append(letters.charAt(rand.nextInt(letters.length())));
        }

        if (sb.length() != 5) {
            throw new BlogException("The length of the ID is incorrect");
        }

        return sb.toString();
    }

    /**
     * Adds a new blog post to the internal list and writes changes to file.
     *
     * @param date date of the post.
     * @param author author name.
     * @param content text content of the post.
     * @return the created BlogPost object.
     * @throws BlogException if any field is empty or ID generation fails.
     * @throws IOException if writing to file fails.
     */
    public BlogPost addPost(String date, String author, String content)
            throws BlogException, IOException {

        if (author == null || author.trim().isEmpty() ||
            date == null || date.trim().isEmpty() ||
            content == null || content.trim().isEmpty()) {
            throw new BlogException("Pola nie mogą być puste!");
        }

        String id = RandomLetters();
        BlogPost post = new BlogPost(date.trim(), author.trim(), content.trim(), id);
        blogList.add(post);
        writeToFile();

        return post;
    }

    /**
     * Deletes a blog post at the specified index.
     *
     * @param index zero-based index of the post to remove.
     * @throws BlogException if admin mode is disabled or index is invalid.
     * @throws IOException if writing to file fails.
     */
    public void deletePost(int index) throws BlogException, IOException {
        if (adminmode == 0) {
            throw new BlogException("Brak uprawnień administratora.");
        }
        if (index < 0 || index >= blogList.size()) {
            throw new BlogException("Niepoprawny numer wpisu.");
        }
        blogList.remove(index);
        writeToFile();
    }

    /**
     * Deletes all blog posts.
     *
     * @throws BlogException if admin mode is disabled.
     * @throws IOException if writing to file fails.
     */
    public void deleteAllPosts() throws BlogException, IOException {
        if (adminmode == 0) {
            throw new BlogException("Brak uprawnień administratora.");
        }
        blogList.clear();
        writeToFile();
    }

    /**
     * Edits a blog post at a given index.
     *
     * @param index zero-based index of the post.
     * @param author new author name.
     * @param date new date.
     * @param content new text content.
     * @throws BlogException if admin mode is disabled, fields are empty, or index invalid.
     * @throws IOException if writing to file fails.
     */
    public void editPost(int index, String author, String date, String content)
            throws BlogException, IOException {

        if (adminmode == 0) {
            throw new BlogException("Brak uprawnień administratora.");
        }
        if (index < 0 || index >= blogList.size()) {
            throw new BlogException("Niepoprawny indeks posta.");
        }
        if (author == null || author.trim().isEmpty() ||
            date == null || date.trim().isEmpty() ||
            content == null || content.trim().isEmpty()) {
            throw new BlogException("Pola nie mogą być puste!");
        }

        BlogPost post = blogList.get(index);
        post.setAuthor(author.trim());
        post.setDate(date.trim());
        post.setContent(content.trim());

        writeToFile();
    }

    /**
     * Counts how many posts were written by the given author.
     *
     * @param author the author to search for.
     * @return number of posts by that author.
     */
    public int getPostCountByAuthor(String author) {
        if (author == null) return 0;
        int count = 0;
        for (var p : blogList) {
            if (author.equals(p.getAuthor())) count++;
           
        }
        return count;
    }

    /**
     * Reads all blog posts from the storage file and loads them into memory.
     *
     * @throws BlogException if ID generation fails.
     */
    public void readFromFile() throws BlogException {
    File fil = new File(fileName);
    if (!fil.exists()) return;

    try (Scanner myreader = new Scanner(fil)) {
        while (myreader.hasNextLine()) {
            String author = myreader.nextLine();
            if (!myreader.hasNextLine()) break; // brak daty → pomijamy wpis
            String date = myreader.nextLine();

            StringBuilder contentBuilder = new StringBuilder();
            boolean separatorFound = false;

            while (myreader.hasNextLine()) {
                String line = myreader.nextLine();
                if (line.equals(SEPARATOR)) {
                    separatorFound = true;
                    break;
                }
                if (contentBuilder.length() > 0) contentBuilder.append('\n');
                contentBuilder.append(line);
            }

            // dodajemy wpis tylko jeśli separator był znaleziony
            if (separatorFound) {
                String id = RandomLetters();
                blogList.add(new BlogPost(date, author, contentBuilder.toString(), id));
            }
        }
    } catch (FileNotFoundException e) {
        e.printStackTrace();
    }
}


    /**
     * Writes all blog posts to the storage file.
     *
     * @throws IOException if writing to the file fails.
     */
    public void writeToFile() throws IOException {
        try (FileWriter mywriter = new FileWriter(fileName)) {
            for (BlogPost p : blogList) {
                mywriter.write(p.getAuthor() + "\n");
                mywriter.write(p.getDate() + "\n");
                mywriter.write(p.getContent() + "\n");
                mywriter.write(SEPARATOR + "\n");
            }
        }
    }
}
