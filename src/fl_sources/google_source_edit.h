
#include "google_source.h"
#include <glib.h>
/**
 * A structure to indicate an edit to the Google Reader "database".
 * These edits are put in a queue and processed in sequential order
 * so that google does not end up processing the requests in an 
 * unintended order.
 */
typedef struct edit {
	/**
	 * The guid of the item to edit.
	 */
	gchar* guid;

	/**
	 * The feed url to containing the item. Or the url of the subscription
	 * to edit.
	 */
	gchar* feedUrl;	

	/**
	 * A callback function on completion of the edit.
	 */
	void (*callback) (GoogleSourcePtr gsource, struct edit* edit, gboolean success);
	enum { 
		EDIT_ACTION_MARK_READ,
		EDIT_ACTION_MARK_UNREAD,
		EDIT_ACTION_TRACKING_MARK_UNREAD, /**< every UNREAD request, should be followed by tracking-kept-unread */
		EDIT_ACTION_ADD_SUBSCRIPTION,
		EDIT_ACTION_REMOVE_SUBSCRIPTION
	} action;
		
} *editPtr ; 

/**
 * Create a new instance of edit.
 */
editPtr google_source_edit_new () ; 

/**
 * Free an allocated edit structure
 * @param edit the a pointer to the edit structure to delete.
 */
void google_source_edit_free (editPtr edit); 

/**
 * Process the waiting edits on the edit queue. Call this if the state of
 * the GoogleSource has changed.
 * 
 * @param gsource The GoogleSource whose editQueue should be processed.
 */
void google_source_edit_process(GoogleSourcePtr gsource);


/**
 * Push an edit action onto the processing queue. This is
 * a helper function, use google_source_edit_push_safe instead,
 * as this may not work if the GoogleSource is not yet connected.
 * 
 * @param gsource The GoogleSource structure whose queue you want to push
 *               onto.
 * @param edit   The edit to push.
 * @param head   Whether to push the edit to the head.
 */
void
google_source_edit_push (GoogleSourcePtr gsource, editPtr edit, gboolean head) ;


/** Edit wrappers */

/**
 * Mark the given item as read. An item is identified by the guid, and
 * the feed url to which it belongs.
 * 
 * @param gsource The GoogleSource structure 
 * @param guid   The guid of the item whose status is to be edited
 * @param feedUrl  The feedUrl of the feed containing the item.
 * @param newStatus The new read status of the item (TRUE for read)
 */
void google_source_edit_mark_read(
	GoogleSourcePtr gsource, 
	const gchar* guid, 
	const gchar* feedUrl, 
	gboolean newStatus);


/**
 * Add a subscription to the google source.
 *
 * @param gsource The GoogleSource structure
 * @param feedUrl the feed to add
 */
void google_source_edit_add_subscription(GoogleSourcePtr gsource, const gchar* feedUrl);


/**
 * Remove a subscription from the google source.
 * 
 * @param gsource The GoogleSource structure
 * @param feedUrl the feed to remove
 */
void google_source_edit_remove_subscription(GoogleSourcePtr gsource, const gchar* feedUrl) ;

/**
 * See if an item with give guid is being modified 
 * in the queue.
 *
 * @param GoogleSource the GoogleSource structure
 * @param guid the guid of the item
 */
gboolean google_source_edit_is_in_queue(GoogleSourcePtr gsource, const gchar* guid) ;