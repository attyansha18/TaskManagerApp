import tkinter as tk
from tkinter import messagebox, simpledialog, ttk

tasks = []

class Task:
    def __init__(self, title, desc, priority, completed=False):
        self.title = title
        self.desc = desc
        self.priority = priority
        self.completed = completed

    def __str__(self):
        status = "✅" if self.completed else "❌"
        return f"{status} {self.title} [{self.priority}]"

def refresh_task_list():
    listbox.delete(0, tk.END)
    sorted_tasks = sorted(tasks, key=lambda x: x.priority)
    for task in sorted_tasks:
        listbox.insert(tk.END, str(task))

def add_task():
    title = simpledialog.askstring("Title", "Enter task title:")
    desc = simpledialog.askstring("Description", "Enter task description:")
    priority = simpledialog.askinteger("Priority", "1 = High, 2 = Medium, 3 = Low", minvalue=1, maxvalue=3)

    if title and priority:
        tasks.append(Task(title, desc, priority))
        refresh_task_list()

def mark_completed():
    selected = listbox.curselection()
    if selected:
        idx = selected[0]
        sorted_tasks = sorted(tasks, key=lambda x: x.priority)
        sorted_tasks[idx].completed = True
        refresh_task_list()
    else:
        messagebox.showwarning("No Selection", "Select a task to mark as completed.")

def delete_task():
    selected = listbox.curselection()
    if selected:
        idx = selected[0]
        sorted_tasks = sorted(tasks, key=lambda x: x.priority)
        task_to_delete = sorted_tasks[idx]
        tasks.remove(task_to_delete)
        refresh_task_list()
    else:
        messagebox.showwarning("No Selection", "Select a task to delete.")

# --- UI Setup ---
root = tk.Tk()
root.title("🗒️ Task Manager")

frame = tk.Frame(root, padx=10, pady=10)
frame.pack(fill="both", expand=True)

# Scrollbar
scrollbar = tk.Scrollbar(frame)
scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

# Task List
listbox = tk.Listbox(frame, height=15, width=50, font=("Arial", 12))
listbox.pack(side=tk.LEFT, fill="both", expand=True)
listbox.config(yscrollcommand=scrollbar.set)
scrollbar.config(command=listbox.yview)

# Buttons
btn_frame = tk.Frame(root)
btn_frame.pack(pady=10)

tk.Button(btn_frame, text="➕ Add Task", command=add_task, width=15).grid(row=0, column=0, padx=5)
tk.Button(btn_frame, text="✅ Mark Completed", command=mark_completed, width=15).grid(row=0, column=1, padx=5)
tk.Button(btn_frame, text="❌ Delete Task", command=delete_task, width=15).grid(row=0, column=2, padx=5)

# Start App
refresh_task_list()
root.mainloop()
