package com.mycompany.weblab5;

import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.*;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * View servlet for displaying the blog history and reading cookies.
 * * @author Bartosz Mikosz
 * @version 1.5
 */
@WebServlet("/view")
public class ViewServlet extends HttpServlet {

    protected void processRequest(HttpServletRequest request, HttpServletResponse response) throws IOException {
        BlogModel model = (BlogModel) getServletContext().getAttribute("sharedModel");
        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();

        // Odczyt ciasteczka
        String lastAuthor = "Gość";
        Cookie[] cookies = request.getCookies();
        if (cookies != null) {
            for (Cookie c : cookies) {
                if ("lastAuthor".equals(c.getName())) lastAuthor = c.getValue().replace("_", " ");
            }
        }

        out.println("<html><body><h1>Blog Archive</h1>");
        out.println("<p>Ostatni autor (z cookie): <b>" + lastAuthor + "</b></p>");

        // Status Admina
        if (model.getAdminmode() == 1) {
            out.println("<p style='color:green;'>TRYB ADMINA AKTYWNY | <a href='manage?action=clearAll'>USUN WSZYSTKIE POSTY</a></p>");
        } else {
            out.println("<form action='manage' method='POST'>Admin Pass: <input type='password' name='password'>"
                      + "<input type='hidden' name='action' value='login'><input type='submit' value='Login'></form>");
        }

        // Lista postów
        int i = 0;
        for (BlogPost p : model.getBlogList()) {
            String pid = p.getCode().uniquecode();
            out.println("<div style='border-bottom:1px solid #000;'>");
            out.println("<p>[" + pid + "] <b>" + p.getAuthor() + "</b> (" + p.getDate() + ")</p>");
            out.println("<pre>" + p.getContent() + "</pre>");
            if (model.getAdminmode() == 1) {
                out.println("<a href='manage?action=delete&id=" + i + "'>Usuń ten post</a>");
                // Link do edycji – przesyłamy indeks i nowe (przykładowe) dane
                out.println("<a href='edit.html?id=" + i + "'>Edytuj</a>");
            }
            out.println("</div>");
            i++;
        }
        out.println("<br><a href='index.html'>Dodaj nowy post</a></body></html>");
    }

    @Override protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws IOException { processRequest(req, resp); }
    @Override protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws IOException { processRequest(req, resp); }
}