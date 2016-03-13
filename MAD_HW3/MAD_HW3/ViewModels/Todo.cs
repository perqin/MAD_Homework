using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Media.Imaging;

namespace MAD_HW3.ViewModels
{
    public class Todo : INotifyPropertyChanged
    {
        private string title;
        private string detail;
        private DateTime dueDate;
        private ImageSource coverSource;
        private bool? done;

        public string Title {
            get
            {
                return title;
            }
            set
            {
                title = value;
                OnPropertyChanged();
            }
        }
        public string Detail
        {
            get
            {
                return detail;
            }
            set
            {
                detail = value;
                OnPropertyChanged();
            }
        }
        public DateTime DueDate
        {
            get
            {
                return dueDate;
            }
            set
            {
                dueDate = value;
                OnPropertyChanged();
            }
        }
        public ImageSource CoverSource {
            get {
                return coverSource;
            }
            set {
                coverSource = value;
                OnPropertyChanged();
            }
        }
        public bool? Done
        {
            get
            {
                return done;
            }
            set {
                done = value;
                OnPropertyChanged();
            }
        }

        public Todo()
        {
            title = "New Todo";
            detail = "Detail here...";
            done = false;
            coverSource = new BitmapImage(new Uri("ms-appx://MAD_HW3/Assets/default.png"));
            dueDate = DateTime.Today;
        }

        public event PropertyChangedEventHandler PropertyChanged = delegate { };

        public void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    /// <summary>
    /// Singleton class for access of Todo data.
    /// </summary>
    public class TodoViewModel
    {
        private static TodoViewModel _instance;

        private ObservableCollection<Todo> todos = new ObservableCollection<Todo>();

        public ObservableCollection<Todo> Todos { get { return todos; } }

        public static TodoViewModel getInstance()
        {
            if (_instance == null)
            {
                _instance = new TodoViewModel();
            }
            return _instance;
        }

        public void addTodo(Todo todo)
        {
            todos.Add(todo);
        }
    }

    /// <summary>
    /// Boolean to Visibility converter
    /// </summary>
    /// <seealso cref="https://github.com/jamesmcroft/WinUX-UWP-Toolkit/blob/master/Croft.Core/Croft.Core.UWP/Xaml/Converters/BooleanToVisibilityConverter.cs"/>
    public class BooleanToVisibilityConverter : IValueConverter
    {
        /// <summary>
        /// Converts a bool value to a Visibility value.
        /// </summary>
        /// <returns>
        /// Returns Visibility.Visible if true, else Visibility.Collapsed.
        /// </returns>
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            var b = value as bool?;
            return b == null ? Visibility.Collapsed : (b.Value ? Visibility.Visible : Visibility.Collapsed);
        }

        /// <summary>
        /// Converts a Visibility value to a bool value.
        /// </summary>
        /// <returns>
        /// Returns true if Visiblility.Visible, else false.
        /// </returns>
        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            var v = value as Visibility?;
            return v == null ? (object)null : v.Value == Visibility.Visible;
        }
    }

    public class DateTimeToOffsetConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            return (DateTimeOffset)((value as DateTime?) ?? DateTime.Today);
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            return ((value as DateTimeOffset?) ?? DateTimeOffset.Now).DateTime;
        }
    }
}
