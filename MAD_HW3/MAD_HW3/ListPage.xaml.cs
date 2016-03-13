using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

using MAD_HW3.ViewModels;
using System.Diagnostics;

namespace MAD_HW3
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class ListPage : Page
    {
        private TodoViewModel TodoVM;

        public ListPage()
        {
            this.InitializeComponent();

            TodoVM = TodoViewModel.getInstance();

            // TO BE DELETED
            TodoVM.addTodo(new Todo { Title = "Title1", Done = true });
            TodoVM.addTodo(new Todo { Title = "Title2", Done = true });
            TodoVM.addTodo(new Todo { Title = "Title3", Done = false });
            TodoVM.addTodo(new Todo { Title = "Title4", Done = true });


        }

        private void FFFk(object sender, RoutedEventArgs e)
        {
            // CheckBox Click
        }

        private void TodoListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Debug.WriteLine("TodoListView_SelectionChanged");
        }

        private void TodoListView_ItemClick(object sender, ItemClickEventArgs e)
        {
            Frame rootFrame = Window.Current.Content as Frame;
            MainPage mainPage = rootFrame.Content as MainPage;
            mainPage.OnTodoItemClick(sender, e);
        }
    }
}
