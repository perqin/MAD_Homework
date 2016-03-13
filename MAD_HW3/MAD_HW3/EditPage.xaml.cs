using Windows.UI.Xaml.Controls;

using MAD_HW3.ViewModels;

namespace MAD_HW3
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class EditPage : Page
    {
        private TodoViewModel TodoVM;
        private Todo displayTodo = new Todo();

        public EditPage()
        {
            this.InitializeComponent();

            TodoVM = TodoViewModel.getInstance();
        }

        public void UpdateView(Todo todo)
        {
            displayTodo.Title = todo.Title;
            displayTodo.Detail = todo.Detail;
            displayTodo.DueDate = todo.DueDate;
            displayTodo.Done = todo.Done;
            displayTodo.CoverSource = todo.CoverSource;
            //RefreshView();
        }

        //private void RefreshView()
        //{
            //
        //}
    }
}
