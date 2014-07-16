__author__ = 'Craig Cuninghame'

import sqlite3

#####   FUNCTION TO ADD DEVICES   #####
def add_device():
    #Input device ID
    device_id = raw_input("Enter the Device ID (Written on device): ")
    #Input device Location
    location = raw_input("Enter the Device Location (e.g. Kitchen / Bathroom etc): ")
    #Input device name
    name = raw_input("Enter the Device Name (e.g. TV Cabinet, Bedroom Light etc): ")
    #Input device type
    print ("Enter the device type:")
    print ('''
    1 = Lighting
    2 = Power Switch
    3 = Sensor
    4 = Other
    ''')
    ##Future work - standard devices with multiple sensor / switching arrangments
    type = int(raw_input("Please enter your choice: "))
    if type == 1:
        type = "LTGSWITCH"
    elif type == 2:
        type = "PWRSWITCH"
    elif type == 3:
        type = "SENSOR"
    else:
        type = "OTHER"

    #Populate other fields with default values
    status = "Unknown"
    reading = "Unknown"
    flag_1 = "N"
    flag_2 = "N"

    do_continue = raw_input("Do you wish to add device to database? (Y / N): ")
    if do_continue == "Y" or "y":
        write_database(device_id, location, name, type, status, reading, flag_1, flag_2)
        print ("Device Added!!!")
    else:
        print ("Device NOT Added!!!")
        exit()


#####     FUNCTION TO INITIALISE DATABASE     #####
def setup_database():
    try:
        #Creates or opens a file called mydb with a SQlite3 DB
        db = sqlite3.connect("cdc_database")
        #get a cursor object
        cursor = db.cursor()
        #check if table users does not exist and create it
        cursor.execute('''CREATE TABLE IF NOT EXISTS overview(id INTEGER PRIMARY KEY, device_id INTEGER,
        location TEXT, name TEXT, type TEXT, status TEXT, reading INTEGER, flag_1 TEXT, flag_2 TEXT )''')
        cursor.execute('''CREATE TABLE IF NOT EXISTS log(log_no INTEGER PRIMARY KEY, time TEXT, device_id INTEGER,
        location TEXT, name TEXT, type TEXT, status TEXT, reading INTEGER)''')
        #commit the change
        db.commit()

    #catch the exception
    except Exception as e:
        #roll back any change if something goes wrong
        db.rollback()
        raise e

    finally:
        #close the db connection
        db.close()






#####   FUNCTION TO ADD INDIVIDUAL LINES TO THE DATABASE    #####
def write_database(device_id, location, name, type, status, reading, flag_1, flag_2):
    try:
        #Creates or opens a file called mydb with a SQlite3 DB
        db = sqlite3.connect("cdc_database")
        #get a cursor object
        cursor = db.cursor()
        cursor.execute('''INSERT INTO overview(device_id, location, name, type, status, reading, flag_1, flag_2)
        VALUES (?,?,?,?,?,?,?,?)''', (device_id, location, name, type, status, reading, flag_1, flag_2))
        #commit the change
        db.commit()

    #catch the exception
    except Exception as e:
        #roll back any change if something goes wrong
        db.rollback()
        raise e

    finally:
        #close the db connection
        db.close()


#####   FUNCTION TO DELETE DEVICE FROM THE DATABASE #####
def del_device():
    #Creates or opens a file called cdc_database with a SQlite3 DB
    db = sqlite3.connect("cdc_database")
    #get a cursor object
    cursor = db.cursor()
    cursor.execute('''SELECT id, device_id, location, name FROM overview''')
    for row in cursor:
        print('{0} : {1} : {2} : {3}'.format(row[0], row[1], row[2], row[3]))
    to_be_deleted = raw_input("Enter the number of the device to be removed:")
    cursor.execute('''SELECT id, device_id, location, name FROM overview WHERE id=?''', (to_be_deleted))
    for row in cursor:
        print("The following device will be deleted: " + "{0} : {1} : {2} : {3}".format(row[0], [1], [2], [3]))
    do_continue = raw_input("Do you wish to continue Y / N")
    if do_continue == "Y" or "y":
        try:
            #Creates or opens a file called mydb with a SQlite3 DB
            db = sqlite3.connect("cdc_database")
            #get a cursor object
            cursor = db.cursor()
            #delete the device
            cursor.execute('''DELETE FROM overview WHERE id = ?''',(to_be_deleted))
            #commit the change
            db.commit()
            print ("The device was deleted!!!")

        #catch the exception
        except Exception as e:
            #roll back any change if something goes wrong
            db.rollback()
            raise e

        finally:
            #close the db connection
            db.close()


def list_database():
    #Creates or opens a file called cdc_database with a SQlite3 DB
    db = sqlite3.connect("cdc_database")
    #get a cursor object
    cursor = db.cursor()
    cursor.execute('''SELECT id, device_id, location, name FROM overview''')
    for row in cursor:
        print('{0} : {1} : {2} : {3}'.format(row[0], row[1], row[2], row[3]))


def main():
    print ('''
    ##### Welcome to the Craig Home Automation Setup Screen #####

    Please choose from the following options:
    1) Setup the Database (first run only)
    2) Add devices to the Database
    3) Remove a device from the Database
    4) List the items in the Database
    5) EXIT
    ''')
    choice = raw_input("Please enter your choice: ")
    if choice == "1":
        setup_database()
        #add delay
        main()
    elif choice == "2":
        add_device()
        #add delay
        main()
    elif choice == "3":
        del_device()
        #add delay
        main()
    elif choice == "4":
        list_database()
        #add delay
        main()
    elif choice == "5":
        exit()
    else:
        exit()


if __name__ == '__main__':
    main()

