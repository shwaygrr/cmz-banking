# cmz-banking

## Overview

The `cmz-banking` project is a secure banking application designed to handle user accounts, transactions, and activities with a focus on strong cryptographic practices. The application implements features such as password hashing with salting, digital signatures, and AES encryption to ensure data security.

## Features

- **User Management**: Create and manage user accounts with secure password storage.
- **Transactions**: Handle financial transactions between user accounts with digital signatures for integrity.
- **Activities Log**: Track user activities with timestamps and descriptions.
- **Cryptographic Security**: Utilize AES for encryption and RSA for digital signatures.

## Requirements

- **Qt Framework**: Required for building and running the application.
- **C++ Compiler**: Ensure a compatible C++ compiler is available.

## Installation

1. **Clone the Repository**

    ```bash
    git clone https://your-repository-url.git
    cd cmz-banking
    ```

2. **Install Dependencies**

    Ensure that Qt is installed on your system. You can download it from [Qt's official website](https://www.qt.io/download).

3. **Build the Project in QtCreator**

    - Open Qt Creator.
    - `Build` to compile the project.

      ```
      Ctrl + b
      ```

## Running the Application

1. **Start the Application**

    Navigate to the build directory and run the executable:

    ```bash
    Ctrl + r
    ```

2. **Usage**

    - **Create User**: Use the UI to create a new user account.
    - **Manage Accounts**: Add or delete bank accounts.
    - **Make Transactions**: Transfer money between accounts.
    - **View Activities**: Check the log of user activities.

## Digital Signature and Encryption

The application employs RSA for digital signatures and AES for encryption:

- **RSA**: Used for signing transaction data to ensure integrity.
- **AES-128**: Used for encrypting sensitive data such as passwords.

## Database

The `cmz-banking` application uses an SQLite database to store user data, transactions, and activity logs. The database file is named `db.sqlite` and is located in the `data` folder within the project directory.

### Database Structure

When you first run the application, it will automatically create the necessary tables in the SQLite database. These tables include:

- **Users**: Stores user information, including usernames, password hashes, and salts.
- **Activities**: Logs user activities with timestamps and descriptions.
- **Transactions**: Records financial transactions between user bank accounts.
- **BankAccounts**: Holds all user bank accounts.

### Important Notes

- **Database File**: The database file is named `db.sqlite`. This name is crucial for the application to function correctly. If you rename or move the file, the application will not be able to locate the database and may not work as expected.
- **Location**: The `db.sqlite` file is located in the `data` folder of the project. Ensure that this folder and file are present in the project directory when running the application.

### Initial Setup

On the first run, the application will:
1. Create the `db.sqlite` file in the `data` folder if it does not already exist.
2. Automatically create all the required tables using the SQL schema defined in the application.

Ensure that the `data` folder and `db.sqlite` file are properly set up to avoid any issues with the application's functionality.

