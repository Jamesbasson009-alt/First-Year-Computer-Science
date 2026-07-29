import getpass
from werkzeug.security import generate_password_hash
from database import get_connection

def create_user(username, password, role):
    conn = get_connection()
    c = conn.cursor()
    password_hash = generate_password_hash(password)
    c.execute('''
        INSERT INTO users (username, password_hash, role)
        VALUES (%s, %s, %s)
        ON CONFLICT (username) DO UPDATE SET password_hash = EXCLUDED.password_hash, role = EXCLUDED.role
    ''', (username, password_hash, role))
    conn.commit()
    conn.close()
    print(f"Saved user '{username}' with role '{role}'.")

if __name__ == '__main__':
    print("Create the admin account:")
    admin_username = input("  Admin username: ").strip()
    admin_password = getpass.getpass("  Admin password: ")
    create_user(admin_username, admin_password, 'admin')

    print("\nCreate the receptionist account:")
    recep_username = input("  Receptionist username: ").strip()
    recep_password = getpass.getpass("  Receptionist password: ")
    create_user(recep_username, recep_password, 'receptionist')

    print("\nDone. You can run this script again anytime to reset a password.")
