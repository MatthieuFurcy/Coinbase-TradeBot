import ttkbootstrap as ttk
from ttkbootstrap.constants import *
from tkinter import messagebox

def start_app():
    root = ttk.Window(themename="cosmo")
    app = CoinbaseProBotApp(root)
    root.mainloop()

class CoinbaseProBotApp:
    def __init__(self, root):
        self.root = root
        self.root.title("🚀 CoinbasePro Bot")
        self.root.state('zoomed')

        # Main frames
        self.main_frame = ttk.Frame(root, padding=20)
        self.main_frame.pack(fill="both", expand=True, padx=10, pady=10)

        # Header
        self.header = ttk.Label(self.main_frame, text="🌟 CoinbasePro Trading Bot", font=("Helvetica", 24, "bold"), anchor="center")
        self.header.pack(pady=10)

        # Exchange Status Section
        self.exchange_frame = ttk.LabelFrame(self.main_frame, text="📈 Exchanges", padding=15)
        self.exchange_frame.pack(fill="x", padx=5, pady=10)

        self.exchange_label = ttk.Label(self.exchange_frame, text="Exchange: Coinbase", font=("Helvetica", 12))
        self.exchange_label.grid(row=0, column=0, padx=10, pady=5, sticky="w")

        self.status_label = ttk.Label(self.exchange_frame, text="Status: 🔴 Not Connected", font=("Helvetica", 12))
        self.status_label.grid(row=1, column=0, padx=10, pady=5, sticky="w")

        self.sequence_label = ttk.Label(self.exchange_frame, text="Sequence: N/A", font=("Helvetica", 12))
        self.sequence_label.grid(row=2, column=0, padx=10, pady=5, sticky="w")

        self.connect_button = ttk.Button(self.exchange_frame, text="🔗 Connect", command=self.connect_to_api, bootstyle=SUCCESS)
        self.connect_button.grid(row=3, column=0, pady=10, sticky="w")

        # Settings section
        self.settings_frame = ttk.LabelFrame(self.main_frame, text="⚙️ Settings", padding=15)
        self.settings_frame.pack(fill="x", padx=5, pady=10)

        ttk.Label(self.settings_frame, text="⏱️ Reload Time (seconds):", font=("Helvetica", 12)).grid(row=0, column=0, sticky="w", padx=5, pady=5)
        self.time_entry = ttk.Entry(self.settings_frame, width=20)
        self.time_entry.insert(0, "61")
        self.time_entry.grid(row=0, column=1, padx=5, pady=5)

        ttk.Label(self.settings_frame, text="🔢 WebSocket Messages:", font=("Helvetica", 12)).grid(row=1, column=0, sticky="w", padx=5, pady=5)
        self.websocket_entry = ttk.Entry(self.settings_frame, width=20)
        self.websocket_entry.insert(0, "5000")
        self.websocket_entry.grid(row=1, column=1, padx=5, pady=5)

        ttk.Label(self.settings_frame, text="🔑 API ID:", font=("Helvetica", 12)).grid(row=2, column=0, sticky="w", padx=5, pady=5)
        self.api_id_entry = ttk.Entry(self.settings_frame, width=40)
        self.api_id_entry.grid(row=2, column=1, padx=5, pady=5)

        ttk.Label(self.settings_frame, text="🛡️ API Secret:", font=("Helvetica", 12)).grid(row=3, column=0, sticky="w", padx=5, pady=5)
        self.api_secret_entry = ttk.Entry(self.settings_frame, width=40, show="*")
        self.api_secret_entry.grid(row=3, column=1, padx=5, pady=5)

        ttk.Label(self.settings_frame, text="🔒 API Passphrase:", font=("Helvetica", 12)).grid(row=4, column=0, sticky="w", padx=5, pady=5)
        self.api_passphrase_entry = ttk.Entry(self.settings_frame, width=40)
        self.api_passphrase_entry.grid(row=4, column=1, padx=5, pady=5)

        self.save_button = ttk.Button(self.settings_frame, text="💾 Save Settings", command=self.save_settings, bootstyle=INFO)
        self.save_button.grid(row=5, column=0, columnspan=2, pady=10)

    def connect_to_api(self):
        api_id = self.api_id_entry.get()
        api_secret = self.api_secret_entry.get()
        api_passphrase = self.api_passphrase_entry.get()

        if not api_id or not api_secret or not api_passphrase:
            messagebox.showerror("Error", "Please fill in all API fields.")
            return

        self.status_label.config(text="Status: 🟢 Connected")
        messagebox.showinfo("Info", "Successfully connected to Coinbase API (simulation).")

    def save_settings(self):
        time = self.time_entry.get()
        websocket_count = self.websocket_entry.get()

        messagebox.showinfo("Settings Saved", f"Settings saved:\nReload Time: {time}s\nWebSocket Count: {websocket_count}")
