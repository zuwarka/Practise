import operator # библиотека операторов для сортировки элементов структур
from tkinter import *

def min_a(n, a, *_): # аргументы функции можно игнорировать
    return n * a # за один день - просто умножаем

def min_b(n, a, b, *_): #
    s = (n // 7) * b # учитываем кол-во дней - семь и умножаем целый остаток
    k = n % 7
    if k: # если кол-во дней не кратко семи
        return s + min(b, min_a(k, a)) # то возвращаем минимум (сравнивая тарифы за день и семь дней)
    else:
        return s

def min_c(n, a, b, c): # процесс аналогичен выше, за тем исключением, что идет рассчет по 28 дням
    s = (n // 28) * c
    k = n % 28
    if k:
        return s + min(c, min_b(k, a, b), min_a(k, a))
    else:
        return s

def inputt_and_count_data():
    a = int(EntryA.get())
    b = int(EntryB.get())
    c = int(EntryC.get())
    n = int(EntryN.get())
    EntryA.delete(0, END)
    EntryA.insert(0, a)
    EntryB.delete(0, END)
    EntryB.insert(0, b)
    EntryC.delete(0, END)
    EntryC.insert(0, c)
    EntryN.delete(0, END)
    EntryN.insert(0, n)

    if ((a or b or c or n) < 1 or (a or b or c or n) > 1000):
        root = Tk()
        root.title("Result")
        canvas_m = Canvas(root, width=360, height=50)
        canvas_m.pack()
        frame_m = Frame(root)
        frame_m.place(relx=0.15, rely=0.15, relheight=0.7, relwidth=0.7)
        Label(frame_m, text="Someone from your are not correspond to task").grid()
        return

    list_of_data = [a, b, c]
    a, b, c = map(int, list_of_data)
    calc = [(28 * a, min_a), (4 * b, min_b), (c, min_c)]  # magic
    calc.sort(key=operator.itemgetter(0))

    res_sort = calc[0][1](n, a, b, c)
    disp_tf.insert(0, f'{res_sort}')


window = Tk()
window.title("Ex №26")
canvas = Canvas(window, width=300, height=200)
canvas.pack()
frame = Frame(window)
frame.place(relx=0.15, rely=0.15, relheight=0.7, relwidth=0.7)
label1 = Label(frame, text="Price for one day ").grid(row=0, sticky=W)
label2 = Label(frame, text="Price for seven days ").grid(row=1, sticky=W)
label3 = Label(frame, text="Price for two fortnights ").grid(row=2, sticky=W)
label4 = Label(frame, text="Parking rent (in days) ").grid(row=3, sticky=W)
EntryA = Entry(frame, width=10, font="Roboto 16")
EntryA.grid(row=0, column=1, sticky=E)
EntryB = Entry(frame, width=10, font="Roboto 16")
EntryB.grid(row=1, column=1, sticky=E)
EntryC = Entry(frame, width=10, font="Roboto 16")
EntryC.grid(row=2, column=1, sticky=E)
EntryN = Entry(frame, width=10, font="Roboto 16")
EntryN.grid(row=3, column=1, sticky=E)
res = Button(frame, text="Count your price ", command=inputt_and_count_data)
res.grid(row=4, sticky=W)
disp_tf = Entry(frame, width=10, font="Roboto 16")
disp_tf.grid(row=4, column=1, sticky=W)
window.mainloop()
