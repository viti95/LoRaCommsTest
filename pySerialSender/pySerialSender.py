import tkinter as tk
from tkinter import messagebox
from tkinter import ttk
import serial
import serial.tools.list_ports

class SerialSenderApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Envío Serial 20x4")

        # Etiqueta de instrucciones
        tk.Label(root, text="Introduce tu mensaje (20 caracteres por línea, 4 líneas):").pack(pady=5)

        # Caja de texto multilinea 4x20
        self.textbox = tk.Text(root, height=4, width=20, font=("Courier", 12))
        self.textbox.pack(pady=5)

        # Menú desplegable para puertos serie
        self.port_var = tk.StringVar()
        ports = [port.device for port in serial.tools.list_ports.comports()]
        ports.reverse()
        if not ports:
            ports = ["(No hay puertos)"]
        self.port_var.set(ports[0])
        tk.Label(root, text="Puerto Serie:").pack()

        self.port_dropdown = ttk.Combobox(root, textvariable=self.port_var, values=ports, state="readonly", height=5)
        self.port_dropdown.set(ports[0])
        self.port_dropdown.pack(pady=5)

        # Botón de envío
        tk.Button(root, text="Enviar por Serie", command=self.send_serial).pack(pady=10)

    def send_serial(self):
        lines = self.textbox.get("1.0", "end").splitlines()
        
        message = lines[0]

        port = self.port_var.get()
        if "(No hay puertos)" in port:
            messagebox.showerror("Error", "No se detectó ningún puerto serie.")
            return

        try:
            with serial.Serial(port, 115200, timeout=1) as ser:
                ser.write(message.encode('utf-8'))
                messagebox.showinfo("Enviado", "Mensaje enviado correctamente.")
        except Exception as e:
            messagebox.showerror("Error", f"No se pudo enviar por el puerto {port}:\n{e}")

# Ejecutar la aplicación
if __name__ == "__main__":
    root = tk.Tk()
    app = SerialSenderApp(root)
    root.mainloop()

