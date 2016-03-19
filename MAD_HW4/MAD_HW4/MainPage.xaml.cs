using MAD_HW4.ViewModels;
using System.Diagnostics;
using Windows.ApplicationModel;
using Windows.Foundation;
using Windows.UI.Core;
using Windows.UI.ViewManagement;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace MAD_HW4
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page {
        private int selectedItemIndex = -1;
        private Todo selectedItem = null;

        public MainPage() {
            InitializeComponent();
            Application.Current.Resuming += App_Resuming;
            Application.Current.Suspending += App_Suspending;
            ApplicationView.GetForCurrentView().SetPreferredMinSize(new Size(360, 120));
            SystemNavigationManager.GetForCurrentView().BackRequested += App_BackRequested;
            ListFrame.Navigate(typeof(ListPage));
            EditFrame.Navigate(typeof(EditPage));
        }

        private bool CanGoBack {
            get {
                return EditFrame.Visibility == Visibility.Visible && Grid.GetColumn(EditFrame) == 0;
            }
        }

        private void DeleteButton_Click(object sender, RoutedEventArgs e) {
            EditFrame.Visibility = Visibility.Collapsed;
            GoBack();
            if (selectedItemIndex >= 0) {
                TodoViewModel.getInstance().Todos.RemoveAt(selectedItemIndex);
            }
        }

        private void App_BackRequested(object sender, BackRequestedEventArgs e) {
            Frame rootFrame = Window.Current.Content as Frame;
            if (rootFrame == null) return;

            if (CanGoBack && e.Handled == false) {
                e.Handled = true;
                GoBack();
            }
        }

        private void App_Resuming(object sender, object e)
        {
            Debug.WriteLine("App resume in mainpage");
        }

        private void App_Suspending(object sender, SuspendingEventArgs e)
        {
            Debug.WriteLine("App suspend in mainpage");
        }

        private void GoBack() {
            Grid.SetColumn(EditFrame, 1);
            UpdateButtons();
        }

        private void MainPage_SizeChanged(object sender, SizeChangedEventArgs e) {
            UpdateButtons();
        }

        private void UpdateButtons() {
            SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility = CanGoBack
                ? AppViewBackButtonVisibility.Visible
                : AppViewBackButtonVisibility.Collapsed;
            SaveButton.Visibility = !CanGoBack && EditFrame.Visibility == Visibility.Collapsed
                ? Visibility.Collapsed
                : Visibility.Visible;
            ResetButton.Visibility = !CanGoBack && EditFrame.Visibility == Visibility.Collapsed
                ? Visibility.Collapsed
                : Visibility.Visible;
        }

        public void OnTodoItemClick(object sender, ItemClickEventArgs e) {
            selectedItem = e.ClickedItem as Todo;
            selectedItemIndex = TodoViewModel.getInstance().Todos.IndexOf(selectedItem);

            Frame rootFrame = Window.Current.Content as Frame;
            Grid.SetColumn(EditFrame, rootFrame.ActualWidth > 720 ? 1 : 0);
            EditFrame.Visibility = Visibility.Visible;

            EditPage editPage = EditFrame.Content as EditPage;
            editPage.UpdateView(e.ClickedItem as Todo);

            UpdateButtons();
        }

        public void OnSelectionChanged(int index) {
            DeleteButton.Visibility = index >= 0 ? Visibility.Visible : Visibility.Collapsed;
        }

        private void AddButton_Click(object sender, RoutedEventArgs e) {
            TodoViewModel.getInstance().Todos.Add(new Todo());


            selectedItemIndex = TodoViewModel.getInstance().Todos.Count - 1;
            selectedItem = TodoViewModel.getInstance().Todos[selectedItemIndex];

            ListPage listPage = ListFrame.Content as ListPage;
            listPage.setSelected(selectedItemIndex);

            Frame rootFrame = Window.Current.Content as Frame;
            Grid.SetColumn(EditFrame, rootFrame.ActualWidth > 720 ? 1 : 0);
            EditFrame.Visibility = Visibility.Visible;

            EditPage editPage = EditFrame.Content as EditPage;
            editPage.UpdateView(selectedItem);

            UpdateButtons();
        }

        private void SaveButton_Click(object sender, RoutedEventArgs e) {
            EditPage editPage = EditFrame.Content as EditPage;
            selectedItem.Title = editPage.DisplayTodo.Title;
            selectedItem.Detail = editPage.DisplayTodo.Detail;
            selectedItem.DueDate = editPage.DisplayTodo.DueDate;
            selectedItem.CoverSource = editPage.DisplayTodo.CoverSource;
            selectedItem.Done = editPage.DisplayTodo.Done;
        }

        private void ResetButton_Click(object sender, RoutedEventArgs e) {
            EditPage editPage = EditFrame.Content as EditPage;
            editPage.DisplayTodo.Title = selectedItem.Title;
            editPage.DisplayTodo.Detail = selectedItem.Detail;
            editPage.DisplayTodo.DueDate = selectedItem.DueDate;
            editPage.DisplayTodo.CoverSource = selectedItem.CoverSource;
            editPage.DisplayTodo.Done = selectedItem.Done;
        }
    }
}
