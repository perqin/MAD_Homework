using MAD_HW3.ViewModels;
using Windows.Foundation;
using Windows.UI.Core;
using Windows.UI.ViewManagement;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace MAD_HW3
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        //public delegate void TodoItemClickEventHandler(object sender, ItemClickEventArgs e);
        //public event TodoItemClickEventHandler TodoItemClick;

        public MainPage()
        {
            this.InitializeComponent();

            ApplicationView.GetForCurrentView().SetPreferredMinSize(new Size(360, 120));
            SystemNavigationManager.GetForCurrentView().BackRequested += App_BackRequested;
            ListFrame.Navigate(typeof(ListPage));
            EditFrame.Navigate(typeof(EditPage));
        }

        private bool CanGoBack
        {
            get
            {
                return EditFrame.Visibility == Visibility.Visible && Grid.GetColumn(EditFrame) == 0;
            }
        }

        private void AccClk(object sender, RoutedEventArgs e)
        {
            EditFrame.Visibility = EditFrame.Visibility == Visibility.Visible ? Visibility.Collapsed : Visibility.Visible;
        }

        private void App_BackRequested(object sender, BackRequestedEventArgs e)
        {
            Frame rootFrame = Window.Current.Content as Frame;
            if (rootFrame == null) return;
            
            if (CanGoBack && e.Handled == false)
            {
                e.Handled = true;
                GoBack();
            }
        }

        private void GoBack()
        {
            Grid.SetColumn(EditFrame, 1);
            UpdateBackButton();
        }

        private void MainPage_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            UpdateBackButton();
        }

        private void UpdateBackButton()
        {
            SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility = CanGoBack
                ? AppViewBackButtonVisibility.Visible
                : AppViewBackButtonVisibility.Collapsed;
        }

        public void OnTodoItemClick(object sender, ItemClickEventArgs e)
        {
            EditPage editPage = EditFrame.Content as EditPage;
            editPage.UpdateView(e.ClickedItem as Todo);
        }
    }
}
