#include "Egypt.h"

using namespace std;

void Egypt::check_way()
{
    int n, m, stolica;

    cin >> n;
    cin >> m;
    cin >> stolica;
    --stolica;

    if ((n < 2 || n > 100) || (m < 1 || m > 4950))
    {
        cout << "Your data is invalid\n";
        exit(-1);
    }
    
    vector< map< int, int > > channels(n);
    //create edges between nodes
    for (int edge = 0; edge < m; ++edge)
    {
        int a, b;

        cin >> a;
        cin >> b;
        channels[a - 1][b - 1] = edge;
        channels[b - 1][a - 1] = edge;
    }
    //check alone node
    for (size_t i = 0; i < n; ++i)
    {
        if (channels[i].size() % 2 == 1)
        {
            cout << "There's no way";
            exit(-1);
        }
    }
    //
    vector< int > res(m); //результирующий вектор с путями
    stack< pair< int, int> > st; //стек пар (столица и путь)
    st.push(make_pair(stolica, -1)); //заносим значение в стек
    int count = 0, t;

    while (!st.empty()) //разбираем стек пока он не станет пуст
    {
        int elem = st.top().first;

        if (channels[elem].empty())
        {
            if ((t = st.top().second) != -1)
            {
                res[t] = ++count;
            }
            st.pop();
        }
        else
        {
            int p = channels[elem].begin()->first, num = channels[elem].begin()->second;

            channels[elem].erase(p);
            channels[p].erase(elem);
            st.push(make_pair(p, num));
        }
    }

    if (count != m)
    {
        cout << "There's no way";
    }
    else
    {
        for (size_t i = m; i > 0; i--)
        {
            cout << res[i-1] << "\n";
        }
    }
}
