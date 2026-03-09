package com.mycompany.weblab5;

import jakarta.servlet.ServletContextEvent;
import jakarta.servlet.ServletContextListener;
import jakarta.servlet.annotation.WebListener;

/**
 * Initializes the blog model and stores it in the servlet context.
 * * @author Bartosz Mikosz
 * @version 1.5
 */
@WebListener
public class BlogInitializer implements ServletContextListener {

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        BlogModel model = new BlogModel();
        // Pobieranie ścieżki do pliku w WEB-INF
        String path = sce.getServletContext().getRealPath("/WEB-INF/listablog.txt");
        model.setFileName(path);
        try {
            model.readFromFile();
        } catch (Exception e) {
            sce.getServletContext().log("Initial data load failed", e);
        }
        // Udostępnienie obiektu wszystkim serwletom
        sce.getServletContext().setAttribute("sharedModel", model);
    }
}
