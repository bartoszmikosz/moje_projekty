<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<%@ page import="java.util.*" %>
<%@ page import="pl.polsl.model.Gender" %>

<%    
    String lang = (String) session.getAttribute("lang");
    Locale locale = (lang != null && lang.equals("pl")) ? Locale.of("pl") : Locale.ENGLISH;
    ResourceBundle bundle = ResourceBundle.getBundle("messages", locale);
    Gender[] genders = Gender.values();
%>

<!DOCTYPE html>
<html lang="<%= locale.getLanguage() %>">
    <head>
        <title><%= bundle.getString("title") %></title>
        <script src="js/people.js"></script>
        <script src="js/validator.js"></script>
        <link rel="stylesheet" href="css/main.css">
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    </head>
    <body onload="setDatePicker('dateOfBirth'); getAllPeopleTable('tablePeopleId');">
        <h1><%= bundle.getString("title") %></h1>
        <h2><%= bundle.getString("subtitle") %></h2>
        <div id="errorInfo" class="errorInfo">&nbsp;</div>
        <hr/>
        
        <table>
            <thead>
                <tr>
                    <th><%=  bundle.getString("id") %></th>
                    <th><%=  bundle.getString("name") %></th>
                    <th><%=  bundle.getString("surname") %></th>
                    <th><%=  bundle.getString("email") %></th>
                    <th><%=  bundle.getString("vip") %></th>
                    <th><%=  bundle.getString("gender") %></th>
                    <th><%=  bundle.getString("dateOfBirth") %></th>
                    <th />
                    <th />
                </tr>
            </thead>
            <tbody id="tablePeopleId">                
            </tbody>
        </table>        
        <hr/>
        
        <input type="text" id="name" name="name" placeholder="<%=  bundle.getString("name") %>"/>
        <input type="text" id="surname" name="surname" placeholder="<%=  bundle.getString("surname") %>"/>
        <input type="text" id="email" name="email" placeholder="<%=  bundle.getString("email") %>"/>
        <input type="checkbox" id="vip" name="vip" />
        <select id="gender" name="gender">
            <% for (Gender g : genders) { %>
                <option value="<%= g.name() %>"><%= g.getDisplayName(locale) %></option>
            <% } %>
        </select>
        <input type="date" id="dateOfBirth" name="dateOfBirth" />
        <input type="button" id="add" class="btn-blue-small" value="<%= bundle.getString("add") %>" onClick="insertPerson('name', 'surname', 'email', 'vip', 'gender', 'dateOfBirth', 'tablePeopleId','errorInfo');" />
        <hr/>
        <a id="attendanceListButton" class="btn-green" href="attendanceList.jsp"><%= bundle.getString("attendanceList") %></a>
        <button class="btn-green" onclick="history.back()"><%= bundle.getString("back") %></button>
    </body>
</html>