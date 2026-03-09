package ModelClassJUnitTest;

import com.mycompany.weblab5.*;
import org.junit.jupiter.api.*;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.*;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Testy jednostkowe dla BlogModel:
 * - wszystkie publiczne metody (bez konstruktorów, getterów/setterów)
 * - sytuacje poprawne, błędne i graniczne
 * - parametryzowane testy dla null i pustych wartości
 * - testy enum Column
 * - testy read/write file z plikiem tymczasowym
 */
public class JUnitModelTest {

    private BlogModel model;
    private Path tempFile;

    @BeforeEach
    public void setup() throws IOException {
        model = new BlogModel();
        model.setAdminmode(1); // ustawiamy admina

        // Tworzymy tymczasowy plik
        tempFile = Files.createTempFile("blogtest", ".txt");

        // Ustawiamy w modelu nazwę pliku testowego
        model.setFileName(tempFile.toAbsolutePath().toString());
    }

    // ============================================
    // ENUM Column – testy kompletne
    // ============================================
    @ParameterizedTest
    @EnumSource(Column.class)
    public void ColumnEnum_ShouldContainAllValues(Column c) {
        assertNotNull(c);
    }

    @ParameterizedTest
    @ValueSource(strings = {"INVALID", "XYZ", "AUTH"})
    public void ColumnEnum_ShouldThrowOnInvalidValue(String val) {
        assertThrows(IllegalArgumentException.class, () -> Column.valueOf(val));
    }

    // ============================================
    // RandomLetters – testy
    // ============================================
    @RepeatedTest(5)
    public void RandomLetters_ShouldReturn5Letters() throws BlogException {
        String id = model.RandomLetters();
        assertNotNull(id);
        assertEquals(5, id.length());
    }

    // ============================================
    // addPost – testy poprawne
    // ============================================
    @ParameterizedTest
    @CsvSource({
            "2024-01-01, Adam, Test content",
            "2023-12-11, Beata, Hello world"
    })
    public void addPost_ShouldAddCorrectPost(String date, String author, String content)
            throws BlogException, IOException {
        BlogPost post = model.addPost(date, author, content);
        assertNotNull(post);
        assertEquals(author, post.getAuthor());
        assertEquals(date, post.getDate());
        assertEquals(content, post.getContent());
        assertEquals(1, model.getBlogList().size());
    }

    // addPost – testy błędne (null i pusty string)
    @ParameterizedTest
    @CsvSource({
            "null, Adam, Content",
            "2024-01-01, null, Content",
            "2024-01-01, Adam, null",
            "'', Adam, Content",
            "2024-01-01, '', Content",
            "2024-01-01, Adam, ''"
    })
    public void addPost_ShouldThrowOnInvalidInput(String date, String author, String content) {
        if ("null".equals(date)) date = null;
        if ("null".equals(author)) author = null;
        if ("null".equals(content)) content = null;

        final String finalDate = date;
        final String finalAuthor = author;
        final String finalContent = content;

        assertThrows(BlogException.class, () -> model.addPost(finalDate, finalAuthor, finalContent));
    }

    // ============================================
    // deletePost i deleteAllPosts
    // ============================================
    @Test
    public void deletePost_ShouldRemovePost() throws BlogException, IOException {
        model.addPost("2024-01-01", "Adam", "Content1");
        model.addPost("2024-01-02", "Beata", "Content2");

        assertEquals(2, model.getBlogList().size());

        model.deletePost(0);
        assertEquals(1, model.getBlogList().size());
        assertEquals("Beata", model.getBlogList().get(0).getAuthor());
    }

    @ParameterizedTest
    @ValueSource(ints = {-1, 5})
    public void deletePost_ShouldThrowOnInvalidIndex(int index) {
        assertThrows(BlogException.class, () -> model.deletePost(index));
    }

    @Test
    public void deleteAllPosts_ShouldClearList() throws BlogException, IOException {
        model.addPost("2024-01-01", "Adam", "Content1");
        model.addPost("2024-01-02", "Beata", "Content2");

        model.deleteAllPosts();
        assertTrue(model.getBlogList().isEmpty());
    }

    // ============================================
    // editPost
    // ============================================
    @Test
    public void editPost_ShouldEditPost() throws BlogException, IOException {
        model.addPost("2024-01-01", "Adam", "Content1");
        model.editPost(0, "Beata", "2024-02-02", "New content");

        BlogPost post = model.getBlogList().get(0);
        assertEquals("Beata", post.getAuthor());
        assertEquals("2024-02-02", post.getDate());
        assertEquals("New content", post.getContent());
    }

    @ParameterizedTest
    @CsvSource({
            "-1, Adam, 2024-01-01, Content",
            "0, null, 2024-01-01, Content",
            "0, Adam, null, Content",
            "0, Adam, 2024-01-01, null",
            "0, '', 2024-01-01, Content"
    })
    public void editPost_ShouldThrowOnInvalidInput(int index, String author, String date, String content) {
        if ("null".equals(author)) author = null;
        if ("null".equals(date)) date = null;
        if ("null".equals(content)) content = null;

        final int finalIndex = index;
        final String finalAuthor = author;
        final String finalDate = date;
        final String finalContent = content;

        assertThrows(BlogException.class, () -> model.editPost(finalIndex, finalAuthor, finalDate, finalContent));
    }

    // ============================================
    // getPostCountByAuthor
    // ============================================
    @Test
    public void getPostCountByAuthor_ShouldReturnCorrectCount() throws BlogException, IOException {
        model.addPost("2024-01-01", "Adam", "A");
        model.addPost("2024-01-02", "Adam", "B");
        model.addPost("2024-01-03", "Beata", "C");

        assertEquals(2, model.getPostCountByAuthor("Adam"));
        assertEquals(1, model.getPostCountByAuthor("Beata"));
        assertEquals(0, model.getPostCountByAuthor("Xyz"));
        assertEquals(0, model.getPostCountByAuthor(null));
    }

    // ============================================
    // writeToFile i readFromFile
    // ============================================
    @Test
    public void writeAndReadFromFile_ShouldPersistPosts() throws BlogException, IOException {
        model.addPost("2024-01-01", "Adam", "Content1");
        model.addPost("2024-01-02", "Beata", "Content2");

        model.writeToFile();

        model.getBlogList().clear();
        assertTrue(model.getBlogList().isEmpty());

        model.readFromFile();
        assertEquals(2, model.getBlogList().size());
        assertEquals("Adam", model.getBlogList().get(0).getAuthor());
        assertEquals("Beata", model.getBlogList().get(1).getAuthor());
    }

    @Test
    public void readFromFile_ShouldHandleEmptyFile() throws BlogException, IOException {
        Files.write(tempFile, "".getBytes(StandardCharsets.UTF_8));
        model.readFromFile();
        assertTrue(model.getBlogList().isEmpty());
    }

    @Test
    public void readFromFile_ShouldHandleCorruptedFile() throws IOException {
        try (BufferedWriter writer = Files.newBufferedWriter(tempFile)) {
            writer.write("AuthorOnly\n");
            writer.write("DateOnly\n");
            writer.write("Content line1\n");
            // brak separatora
        }

        assertDoesNotThrow(() -> model.readFromFile());
        assertTrue(model.getBlogList().isEmpty());
    }

    @Test
    public void writeToFile_ShouldPersistAndOverwriteFile() throws BlogException, IOException {
        model.addPost("2024-01-01", "Adam", "Content1");
        model.writeToFile();
        String content = Files.readString(tempFile);
        assertTrue(content.contains("Adam"));
        assertTrue(content.contains("Content1"));

        model.addPost("2024-01-02", "Beata", "Content2");
        model.writeToFile();
        content = Files.readString(tempFile);
        assertTrue(content.contains("Adam"));
        assertTrue(content.contains("Beata"));
        assertTrue(content.contains("Content2"));
    }

    @Test
    public void readFromFile_ShouldRestoreMultiplePosts() throws BlogException, IOException {
        model.addPost("2024-01-01", "Adam", "Content1");
        model.addPost("2024-01-02", "Beata", "Content2");
        model.writeToFile();

        model.getBlogList().clear();
        model.readFromFile();

        assertEquals(2, model.getBlogList().size());
        assertEquals("Adam", model.getBlogList().get(0).getAuthor());
        assertEquals("Beata", model.getBlogList().get(1).getAuthor());
    }
}
