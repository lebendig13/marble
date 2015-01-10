//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2014      Calin Cruceru <crucerucalincristian@gmail.com>
//

#ifndef EDITPLACEMARKDIALOG_H
#define EDITPLACEMARKDIALOG_H

#include "marble_export.h"

#include <QDialog>


namespace Marble {

class GeoDataFeature;
class GeoDataPlacemark;


/**
 * @brief The EditTextAnnotationDialog class deals with customizing placemarks.
 */
class MARBLE_EXPORT EditTextAnnotationDialog : public QDialog
{
    Q_OBJECT

public:
    EditTextAnnotationDialog( GeoDataPlacemark *placemark, QWidget *parent = 0 );
    ~EditTextAnnotationDialog();

    /**
     * @brief setFirstTimeEditing tells the dialog whether this is the first time it
     * pops up for this PlacemarkTextAnnotation instance or not.
     */
    void setFirstTimeEditing( bool enabled );

    /**
     * @brief idFilter gets filter for id of placemark
     * @return QStringList of ids which could not be used as id.
     */
    QStringList idFilter() const;

    /**
     * @brief targetIds gets ids which could be target of placemark.
     * @return QStringList of ids which could be target of placemark.
     */
    QStringList targetIds() const;

    /**
     * @brief isTargetIdFieldVisible tells if targetId field is shown.
     */
    bool isTargetIdFieldVisible() const;

    /**
     * @brief isIdFieldVisible tells if targetId field is shown.
     */
    bool isIdFieldVisible() const;

public slots:
    /**
     * @brief updateDialogFields is connected to a signal from AnnotatePlugin in order
     * to update some fields in the dialog as the user interacts directly with the text
     * annotation item.
     */
    void updateDialogFields();

    /**
     * @brief setIdFilter sets filter for id of placemark.
     * @param filter QStringList with ids which could not be used as id.
     */
    void setIdFilter( const QStringList &filter );

    /**
     * @brief setTargetIds sets ids which could be target of placemark.
     * @param targetIds QStringList with ids which could be target of placemark.
     */
    void setTargetIds( const QStringList &targetIds );

    /**
     * @brief setTargetIdFieldVisible tells the dialog wheter targetId field should be shown.
     */
    void setTargetIdFieldVisible( bool visible );

    /**
     * @brief setIdFieldVisible tells the dialog wheter id field should be shown.
     */
    void setIdFieldVisible( bool visible );

    /**
     * @brief Protecting data from input fields changes
     */
    void setReadOnly( bool state );

private slots:

    /**
     * @brief checkFields shows warnings if there are important fields which don't hold
     * accurate information.
     */
    void checkFields();

    /**
     * @brief updateTextAnnotation is the main slot which synchronizes the information
     * from the dialog with the way the text annotation item is painted.
     */
    void updateTextAnnotation();

    /**
     * @brief updateLabelDialog The color chooser for label is represented as a push
     * button with a filled pixmap as its icon. This slot updates the color fill of this
     * pixmap.
     */
    void updateLabelDialog( const QColor &color );

    /**
     * @brief updateIconDialog The same as above, but for icons.
     * FIXME: This is not functional ATM - we need some mechanism for customizing existing
     * icons.
     */
    void updateIconDialog( const QColor &color );

    /**
     * @brief updatePlacemarkAltitude changes an actual elevation value of placemark instance
     * according to the value/unit of elevation widget spin box representing it
     */
    void updatePlacemarkAltitude();

    /**
     * @brief restoreInitial restores the dialog's previous settings if the dialog closes with
     * a zero return value.
     */
    void restoreInitial( int result );

    /**
     * @brief toogleDescriptionEditMode toggles edit mode for description field.
     */
    void toggleDescriptionEditMode( bool isFormattedTextMode = false );

    /**
     * @brief setBold sets bold style for selected text in description field.
     * @param bold
     */
    void setBold( bool bold );

    /**
     * @brief updateDescriptionEditButtons updates stated of buttons for editing description field.
     */
    void updateDescriptionEditButtons();

signals:
    /**
     * @brief textAnnotationUpdated signals that some property of the PlacemarkTextAnnotation
     * instance has changed.
     * @param feature The instance's placemark.
     */
    void textAnnotationUpdated( GeoDataFeature *feature );

private:
    class Private;
    Private * const d;
};

}

#endif