package src;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.DefaultTableModel;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.CallableStatement;
import java.sql.Time;
import java.sql.Date;

import java.util.List;
import java.util.Vector;
import java.util.ArrayList;



public class Main extends JFrame{
    DbConnection connection;
    Settings settings;

    JMenuBar menuBar;
    JMenu databaseMenu;
    JMenuItem connectMenuItem;
    JMenuItem disconnecMenuItem;
    JMenuItem connectionSettingsMenuItem;
    JMenu queryMenu;
    JMenuItem searchItem;
    JMenuItem procItem;

    JTabbedPane tabbedPane;

    Object previousValue = null;
    int previousRow;
    int previousColumn;

    public Main(){
        super();
        settings = new Settings("settings.properties");

        connection = new DbConnection();

        try{
            settings.load();
        } catch (Exception e){
            errorHandler(new Exception("Не удалось загрузить настройки", e));
        }

        String host = settings.getProperty("host");
        connection.setHost(host);
        String port = settings.getProperty("port");
        connection.setPort(port);
        String database = settings.getProperty("database");
        connection.setDatabase(database);
        String username = settings.getProperty("username");
        connection.setUsername(username);
        String password = settings.getProperty("password");
        connection.setPassword(password);

        menuBar = new JMenuBar();

        databaseMenu = new JMenu("База данных");

        connectMenuItem = new JMenuItem("Подключиться");
        connectMenuItem.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e){
                connect();
            }
        });
        databaseMenu.add(connectMenuItem);

        disconnecMenuItem = new JMenuItem("Отключиться");
        disconnecMenuItem.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e){
                disconnect();
            }
        });
        databaseMenu.add(disconnecMenuItem);

        connectionSettingsMenuItem = new JMenuItem("Настройки подключения");
        connectionSettingsMenuItem.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e){
                connectionSettingsDialog();
            }
        });
        databaseMenu.add(connectionSettingsMenuItem);

        menuBar.add(databaseMenu);

        queryMenu = new JMenu("Запросы");
        searchItem = new JMenuItem("Поиск по ИНН");
        searchItem.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e){
                innSearch();
            }
        });
        procItem = new JMenuItem("Выполнить процедуру");
        procItem.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e){
                doProc();
            }
        });
        queryMenu.add(searchItem);
        queryMenu.add(procItem);

        menuBar.add(queryMenu);

        setJMenuBar(menuBar);
    }

    public void initializeTabs(){
        tabbedPane = new JTabbedPane();
        add(tabbedPane, BorderLayout.CENTER);
        tabbedPane.setVisible(true);
    }

    public void clearTabs(){
        tabbedPane.removeAll();
        tabbedPane = null;
    }

    public void refrashTabs(){
        int selected = tabbedPane.getSelectedIndex();
        tabbedPane.removeAll();
        showTables();
        tabbedPane.setSelectedIndex(selected);
    }

    public void connect(){
        try{
            connection.open();
        } catch (Exception e) {
            errorHandler(e);
            return;
        }
        
        if (tabbedPane == null){
            initializeTabs();
        }
        tabbedPane.setVisible(true);
        showTables();
    }

    public void disconnect(){
        tabbedPane.setVisible(false);
        clearTabs();
        try{
            connection.close();
        } catch (Exception ex){
            errorHandler(ex);
        }
    }

    public void errorHandler(Exception e){
        JOptionPane.showMessageDialog(null, "ОШИБКА: " + e);
    }

    private DefaultTableModel getQueryTableModel(String query) throws Exception{
        DefaultTableModel model = new DefaultTableModel();
        try{
            ResultSet resultSet = connection.getQueryResultSet(query);
            ResultSetMetaData resultSetMetaData = resultSet.getMetaData();
            int columnCount = resultSetMetaData.getColumnCount();

            Vector<String> columnNames = new Vector<String>();
            for (int i = 1; i <= columnCount; i++){
                String columnName = resultSetMetaData.getColumnName(i);
                columnNames.add(columnName);
                // model.addColumn(columnName);
            }
            model.setColumnIdentifiers(columnNames);
            // System.out.println(model.getColumnName(0));
            while (resultSet.next()){
                Object[] rowData = new Object[columnCount];
                for (int i = 1; i <= columnCount; i++){
                    Object value = resultSet.getObject(i);
                    rowData[i - 1] = value;
                }
                model.addRow(rowData);
            }
        } catch (Exception e){
            throw new Exception(e.getMessage());
        }
        return model;
    }

    private DefaultTableModel getTableModel(String table) throws Exception{
        DefaultTableModel model = null;
        try{
            model = getQueryTableModel("SELECT * FROM " + table);
            // System.out.println(model.getColumnName(0));
        } catch (Exception e){
            throw new Exception(e.getMessage());
        }
        return model;
    }

    private JTable getTable(String table) throws Exception{
        JTable tableJTable;
        try{
            DefaultTableModel tableModel = getTableModel(table);
            tableJTable = new JTable(tableModel);
        } catch (Exception e){
            throw new Exception(e.getMessage());
        }
        return tableJTable;
    }

    private JPanel getTablePanel(String table) throws Exception{
        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());
        
        try {
            JTable tableJTable = getTable(table);


            tableJTable.getModel().addTableModelListener(new TableModelListener() {
                public void tableChanged(TableModelEvent e){
                    int row = tableJTable.getSelectedRow();
                    try {
                        updateDataInTable(row, table, (DefaultTableModel)tableJTable.getModel());
                    } catch (SQLException ex) {
                        errorHandler(ex);
                    }
                }
            });
            tableJTable.getSelectionModel().addListSelectionListener(new ListSelectionListener() {
                @Override
                public void valueChanged(ListSelectionEvent event){
                    if (!event.getValueIsAdjusting()){
                        previousRow = tableJTable.getSelectedRow();
                        previousValue = tableJTable.getValueAt(previousRow, previousColumn);
                    }
                }
            });
            tableJTable.getColumnModel().getSelectionModel().addListSelectionListener(new ListSelectionListener() {
                @Override
                public void valueChanged(ListSelectionEvent event){
                    if (!event.getValueIsAdjusting()){
                        previousColumn = tableJTable.getSelectedColumn();
                        previousValue = tableJTable.getValueAt(previousRow, previousColumn);
                    }
                }
            });

            panel.add(tableJTable, BorderLayout.CENTER);
        } catch (Exception e){
            throw new Exception(e.getMessage());
        }
        return panel;
    }

    private void updateDataInTable(int row, String tableName, DefaultTableModel tableModel) throws SQLException {
        String sql = "UPDATE " + tableName + " SET ";
        int columnCount = tableModel.getColumnCount();
        String condition = " WHERE ";

        for (int i = 0; i < columnCount; i++) {
            String columnName = tableModel.getColumnName(i);
            Object columnValue = tableModel.getValueAt(row, i);
            sql += columnName + "='" + columnValue + "'";
            condition += tableModel.getColumnName(i) + "='";
            if (i != previousColumn){
                condition += tableModel.getValueAt(row, i) + "'";
            }
            else{
                condition += previousValue.toString() + "'";
            }
            if (i != columnCount - 1) {
                sql += ", ";
                condition += " AND ";
            }
        }
    
        sql += condition;

        try {
            Statement stmt = connection.getStatement();
            stmt.executeUpdate(sql);
        } catch (Exception e){
            errorHandler(e);
        }

        refrashTabs();
    }

    private void showTables(){
        try{
            List<String> tables = connection.getTableNames();
            for (String tableName : tables){
                JPanel tablePanel = getTablePanel(tableName);
                tabbedPane.addTab(tableName, new JScrollPane(tablePanel));
            }
        } catch (Exception e){
            errorHandler(e);
        }
    }

    private void connectionSettingsDialog(){
        JTextField hostTextField = new JTextField(connection.getHost());
        JTextField portTextField = new JTextField(connection.getPort());
        JTextField databaseTextField = new JTextField(connection.getDatabase());
        JTextField usernameTextField = new JTextField(connection.getUsername());
        JTextField passwordTextField = new JTextField(connection.getPassword());

        JComponent[] inputs = new JComponent[] {
            new JLabel("Сервер:"),
            hostTextField,
            new JLabel("Порт:"),
            portTextField,
            new JLabel("База данных:"),
            databaseTextField,
            new JLabel("Имя пользователя:"),
            usernameTextField,
            new JLabel("Пароль:"),
            passwordTextField
        };

        int result = JOptionPane.showConfirmDialog(null, inputs,
        "Данные подключения", JOptionPane.OK_CANCEL_OPTION);

        if (result != JOptionPane.OK_OPTION){
            return;
        }

        String host = hostTextField.getText();
        connection.setHost(host);
        String port = portTextField.getText();
        connection.setPort(port);
        String database = databaseTextField.getText();
        connection.setDatabase(database);
        String username = usernameTextField.getText();
        connection.setUsername(username);
        String password = passwordTextField.getText();
        connection.setPassword(password);

        settings.setProperty("host", host);
        settings.setProperty("port", port);
        settings.setProperty("database", database);
        settings.setProperty("username", username);
        settings.setProperty("password", password);
        try{
            settings.save();
        } catch (Exception e){
            errorHandler(e);
        }
    }

    private void innSearch(){
        JTextField innTextField = new JTextField();
        JComponent[] inputs = new JComponent[]{
            new JLabel("ИНН:"),
            innTextField
        };

        int result = JOptionPane.showConfirmDialog(null, inputs,
        "Введите ИНН для поиска", JOptionPane.OK_CANCEL_OPTION);

        if (result != JOptionPane.OK_OPTION){
            return;
        }

        String inn = innTextField.getText();

        refrashTabs();

        try{
            String query = "SELECT * FROM call WHERE inn='" + inn + "'";
            DefaultTableModel model = getQueryTableModel(query);
            JTable table = new JTable();
            table.setModel(model);
            JPanel panel = new JPanel();
            panel.setLayout(new BorderLayout());
            panel.add(table, BorderLayout.CENTER);
            tabbedPane.addTab("Результаты поиска", new JScrollPane(panel));
        } catch (Exception e){
            errorHandler(e);
        }
    }

    private void doProc(){
        JTextField telephonePointTextField = new JTextField();
        JTextField innTextField = new JTextField();
        JTextField checkingAccaunTextField = new JTextField();
        JTextField timesOfDayTextField = new JTextField();
        JTextField durationTextField = new JTextField();
        JTextField callDateTextField = new JTextField();
        JTextField cityTextField = new JTextField();
        JTextField saleTextField = new JTextField();
        JTextField priceDaytimeTextField = new JTextField();
        JTextField priceNightTextField = new JTextField();

        JComponent[] inputs = new JComponent[]{
            new JLabel("Телефонная точка:"),
            telephonePointTextField,
            new JLabel("ИНН:"),
            innTextField,
            new JLabel("Рассчетный счет:"),
            checkingAccaunTextField,
            new JLabel("Время суток:"),
            timesOfDayTextField,
            new JLabel("Длительность:"),
            durationTextField,
            new JLabel("Дата звонка:"),
            callDateTextField,
            new JLabel("Город:"),
            cityTextField,
            new JLabel("Скидка:"),
            saleTextField,
            new JLabel("Дневной тариф:"),
            priceDaytimeTextField,
            new JLabel("Ночной тариф:"),
            priceNightTextField
        };

        int result = JOptionPane.showConfirmDialog(null, inputs,
        "Введите данные для выполнения процедуры", JOptionPane.OK_CANCEL_OPTION);

        if (result != JOptionPane.OK_OPTION){
            return;
        }

        try{
            String telephonePoint = telephonePointTextField.getText();
            String inn = innTextField.getText();
            String checkingAccaunt = checkingAccaunTextField.getText();
            java.sql.Time timesOfDay = java.sql.Time.valueOf(timesOfDayTextField.getText());
            java.sql.Time duration = java.sql.Time.valueOf(durationTextField.getText());
            java.sql.Date callDate = java.sql.Date.valueOf(callDateTextField.getText());
            String city = cityTextField.getText();
            Double sale = Double.parseDouble(saleTextField.getText());
            Double priceDaytime = Double.parseDouble(priceDaytimeTextField.getText());
            Double priceNight = Double.parseDouble(priceNightTextField.getText());

            String query = "call insert_call(?,?,?,?::time,?::time,?::date,?,?::numeric,?::numeric,?::numeric)";
            CallableStatement callableStatement = 
                                connection.getConnection().prepareCall(query);
            callableStatement.setString(1, telephonePoint);
            callableStatement.setString(2, inn);
            callableStatement.setString(3, checkingAccaunt);
            callableStatement.setTime(4, timesOfDay);
            callableStatement.setTime(5, duration);
            callableStatement.setDate(6, callDate);
            callableStatement.setString(7, city);
            callableStatement.setDouble(8, sale);
            callableStatement.setDouble(9, priceDaytime);
            callableStatement.setDouble(10, priceNight);
            callableStatement.execute();

            refrashTabs();
        } catch (Exception e){
            errorHandler(e);
        }
    }

    public static void main(String[] args) {
        try{
            Main main = new Main();
            main.setBounds(15, 15, 1000, 700);
            main.setTitle("Лабораторная работа № 6");
            main.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            main.setLayout(new BorderLayout());

            final int BORDER_SIZE = 10;
            JPanel[] panels = new JPanel[4];
            String[] directions = { BorderLayout.NORTH, BorderLayout.WEST, BorderLayout.SOUTH, BorderLayout.EAST };
            
            for (int i = 0; i < panels.length; i++) {
                panels[i] = new JPanel(new BorderLayout());
                panels[i].setBorder(BorderFactory.createEmptyBorder(BORDER_SIZE, BORDER_SIZE, BORDER_SIZE, BORDER_SIZE));
                main.add(panels[i], directions[i]);
            }

            main.setVisible(true);
        }
        catch (Exception ex){
            System.err.println("Something went wrong...");
            System.err.println(ex.getMessage());
            System.exit(ABORT);
        }
    }
}