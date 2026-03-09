package com.mycompany.weblab5;

import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.*;
import java.io.IOException;

/**
 * Controller servlet for managing blog operations like adding and deleting posts.
 * * @author Bartosz Mikosz
 * @version 1.5
 */
@WebServlet("/manage")
public class BlogController extends HttpServlet {

    protected void processRequest(HttpServletRequest request, HttpServletResponse response) throws IOException {
        BlogModel model = (BlogModel) getServletContext().getAttribute("sharedModel");
        String action = request.getParameter("action");

        try {
            if ("add".equals(action)) {
                String author = request.getParameter("author");
                if (author == null || author.trim().isEmpty()) throw new BlogException("Author cannot be empty!");
                
                model.addPost(request.getParameter("date"), author, request.getParameter("content"));
                
                // Ciasteczko: zapamiętujemy autora (zamiana spacji na podkreślnik)
                Cookie c = new Cookie("lastAuthor", author.replace(" ", "_"));
                c.setMaxAge(3600);
                response.addCookie(c);
            } 
            else if ("login".equals(action)) {
                if ("admin".equals(request.getParameter("password"))) {
                    model.setAdminmode(1);
                }
            }
            else if ("delete".equals(action) && model.getAdminmode() == 1) {
                String id = request.getParameter("id");
                try {
                    // Teraz id to np. "0", więc parseInt nie rzuci błędu
                    int idnumber = Integer.parseInt(id); 
                    model.deletePost(idnumber);
                } catch (NumberFormatException e) {
                    // Obsługa sytuacji, gdyby ktoś ręcznie wpisał litery w URL
                    response.sendError(HttpServletResponse.SC_BAD_REQUEST, "Indeks musi być liczbą!");
                } catch (BlogException | IOException e) {
                    response.sendError(HttpServletResponse.SC_INTERNAL_SERVER_ERROR, e.getMessage());
                }
            }
            else if ("clearAll".equals(action) && model.getAdminmode() == 1) {
                model.deleteAllPosts();
            }
            
            else if ("edit".equals(action) && model.getAdminmode() == 1) {
            
                String idStr = request.getParameter("id");
                String author = request.getParameter("author");
                String date = request.getParameter("date");
                String content = request.getParameter("content");

                try {
                    int index = Integer.parseInt(idStr);
                    model.editPost(index, author, date, content);
                    response.sendRedirect("view");
                } catch (NumberFormatException | BlogException | IOException e) {
                    response.sendError(HttpServletResponse.SC_BAD_REQUEST, "Błąd edycji: " + e.getMessage());
                }
            }

            response.sendRedirect("view");
        } catch (BlogException e) {
            response.sendError(HttpServletResponse.SC_BAD_REQUEST, "Model error: " + e.getMessage());
        }
    }

    @Override protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws IOException { processRequest(req, resp); }
    @Override protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws IOException { processRequest(req, resp); }
}