/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.weblab5;

/**
 * Exception thrown in case one of model functions fails in any way (used in tests)
 * @author Bartosz Mikosz
 * @version 1.0
 */
public class BlogException extends Exception {
    public BlogException(String message) {
        super(message);
    }
    
     public BlogException(String message, Throwable cause) {
        super(message, cause);
    }
    
}
