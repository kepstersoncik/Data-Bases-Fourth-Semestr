package src;

import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.CallableStatement;
import java.sql.Time;
import java.sql.Date;

import java.util.List;
import java.util.ArrayList;

public class DbConnection {
    String host;
    String port;
    String database;
    String username;
    String password;

    Connection connection;
    Statement statement;

    public DbConnection(){

    }

    public String getHost() {
        return host;
    }

    public void setHost(String host) {
        this.host = host;
    }

    public String getPort() {
        return port;
    }

    public void setPort(String port) {
        this.port = port;
    }

    public String getDatabase() {
        return database;
    }

    public void setDatabase(String database) {
        this.database = database;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void open() throws Exception{
        try {
            if (connection != null){
                if (!connection.isClosed()) return;
            }
            Class.forName("org.postgresql.Driver");
            String connectionString = getConnectionString();
            connection = DriverManager.getConnection(connectionString,
                                               username, password);
            if (statement != null){
                if (!statement.isClosed()) return;
            }
            statement = connection.createStatement();
        } catch (SQLException e) {
            throw new SQLException(e.getMessage());
        } catch (ClassNotFoundException e) {
            throw new ClassNotFoundException("JDBC драйвер не найден");
        } catch (Exception e){
            throw new Exception(e.getMessage());
        }
    }

    public void close() throws Exception{
        try {
            if (connection.isClosed()) return;
            connection.close();

            if (statement.isClosed()) return;
            statement.close();
        } catch (Exception e){
            throw new Exception(e.getMessage());
        }
    }

    public String getConnectionString(){
        return String.format("jdbc:postgresql://%s:%s/%s",
        host, port, database);
    }

    public Connection getConnection() throws Exception{
        boolean isConnectionOpen = !connection.isClosed();
        if (connection != null && isConnectionOpen) return connection;
        try {
            this.open();
        } catch (Exception e){
            throw new Exception(e.getMessage());
        }
        return connection;
    }

    public Statement getStatement() throws Exception{
        boolean isConnectionOpen = !connection.isClosed();
        boolean isStatementOpen = !statement.isClosed();
        if (statement != null && isConnectionOpen && isStatementOpen){
            return statement;
        }
        try{
            this.open();
        } catch (Exception e){
            throw new Exception(e.getMessage());
        }
        return statement;
    }

    public List<String> getTableNames() throws Exception{
        List<String> tables = new ArrayList<>();
        try{
            Connection conn = getConnection();
            DatabaseMetaData metaData = conn.getMetaData();
            ResultSet resultSet = metaData.getTables(null,
            null, "%", new String[]{"TABLE"});

            while (resultSet.next()){
                String tableName = resultSet.getString("TABLE_NAME");
                tables.add(tableName);
            }
        } catch (Exception e){
            throw new Exception(e.getMessage());
        }
        return tables;
    }

    public ResultSet getQueryResultSet(String query) throws Exception{
        ResultSet resultSet = null;
        try{
            Statement statement = getStatement();
            resultSet = statement.executeQuery(query);
        } catch (Exception e){
            throw new Exception(e.getMessage());
        }
        return resultSet;
    }

    public ResultSetMetaData getQueryMetaData(String query) throws Exception{
        ResultSetMetaData metaData = null;
        try{
            ResultSet resultSet = getQueryResultSet(query);
            metaData = resultSet.getMetaData();
        } catch (Exception e){
            throw new Exception(e.getMessage());
        }
        return metaData;
    }
}
